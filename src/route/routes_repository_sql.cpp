#include "routes_repository_sql.h"

#include <QDebug>

#include "route_traits.h"
#include "utils.h"

namespace
{
constexpr char routes[] = "routes";
constexpr char waypoints[] = "waypoints";
constexpr char routeWaypoints[] = "route_waypoints";
} // namespace

using namespace md::domain;

RoutesRepositorySql::RoutesRepositorySql(QSqlDatabase* database, QObject* parent) :
    IRoutesRepository(parent),
    m_routesTable(database, ::routes),
    m_waypointsTable(database, ::waypoints),
    m_routeWaypointsTable(database, ::routeWaypoints)
{
}

RoutesRepositorySql::~RoutesRepositorySql()
{
}

Route* RoutesRepositorySql::route(const QVariant& id) const
{
    QMutexLocker locker(&m_mutex);
    return m_routes.value(id, nullptr);
}

QVariantList RoutesRepositorySql::routeIds() const
{
    QMutexLocker locker(&m_mutex);
    return m_routes.keys();
}

QList<Route*> RoutesRepositorySql::routes() const
{
    QMutexLocker locker(&m_mutex);
    return m_routes.values();
}

QList<const RouteType*> RoutesRepositorySql::routeTypes() const
{
    QMutexLocker locker(&m_mutex);
    return m_routeTypes.values();
}

void RoutesRepositorySql::registerRouteType(const RouteType* routeType)
{
    QMutexLocker locker(&m_mutex);

    if (m_routeTypes.contains(routeType->name))
        return;

    m_routeTypes.insert(routeType->name, routeType);

    for (const WaypointType* wptType : routeType->waypointTypes)
    {
        m_waypointTypes.insert(wptType->name, wptType);
    }

    emit routeTypesChanged();
}

void RoutesRepositorySql::unregisterRouteType(const RouteType* routeType)
{
    QMutexLocker locker(&m_mutex);

    if (!m_routeTypes.contains(routeType->name))
        return;

    m_routeTypes.remove(routeType->name);

    for (const WaypointType* wptType : routeType->waypointTypes)
    {
        m_waypointTypes.remove(wptType->name);
    }

    emit routeTypesChanged();
}

void RoutesRepositorySql::readAll()
{
    QMutexLocker locker(&m_mutex);

    for (const QVariant& routeId : m_routesTable.selectIds())
    {
        if (!m_routes.contains(routeId))
        {
            this->readRoute(routeId);
        }
    }
}

void RoutesRepositorySql::removeRoute(Route* route)
{
    QMutexLocker locker(&m_mutex);

    if (route->id().isNull())
        return;

    // Remove all route waypoints for route
    m_routeWaypointsTable.removeByCondition({ params::route, route->id() });

    // Remove stored waypoints for route, new points will be deleted by parent
    for (Waypoint* waypoint : m_routeWaypoints.values(route))
    {
        this->removeWaypoint(waypoint);
    }
    m_routeWaypoints.remove(route);

    // Remove route
    m_routesTable.removeEntity(route);
    m_routes.remove(route->id());

    emit routeRemoved(route);
    route->deleteLater();
}

void RoutesRepositorySql::restoreRoute(Route* route)
{
    QMutexLocker locker(&m_mutex);

    if (route->id().isNull())
        return;

    // Restore waypoints and delete new points
    auto remover = [](Waypoint* waypoint) {
        waypoint->deleteLater();
    };
    connect(route, &Route::waypointRemoved, this, remover);
    route->setWaypoints(m_waypoints.values());
    disconnect(route, &Route::waypointRemoved, this, nullptr);

    for (Waypoint* waypoint : route->waypoints())
    {
        m_waypointsTable.readEntity(waypoint);
    }

    m_routesTable.readEntity(route);
    emit routeChanged(route);
}

void RoutesRepositorySql::saveRoute(Route* route)
{
    QMutexLocker locker(&m_mutex);

    if (route->id().isNull())
    {
        qWarning() << "Can't save route with no id" << route;
        return;
    }

    // Update or insert route
    if (m_routes.contains(route->id()))
    {
        m_routesTable.updateEntity(route);
        emit routeChanged(route);
    }
    else
    {
        m_routesTable.insertEntity(route);
        m_routes.insert(route->id(), route);

        route->moveToThread(this->thread());
        route->setParent(this);

        emit routeAdded(route);
    }

    // Remove deleted waypoints
    for (Waypoint* waypoint : m_routeWaypoints.values(route))
    {
        if (route->waypoints().contains(waypoint))
            continue;

        m_routeWaypointsTable.removeByConditions(
            { { params::route, route->id() }, { params::waypoint, waypoint->id() } });
        this->removeWaypoint(waypoint);
        m_routeWaypoints.remove(route, waypoint);
    }

    //  Update or insert waypoints
    for (Waypoint* waypoint : route->waypoints())
    {
        if (m_waypoints.contains(waypoint->id()))
        {
            m_waypointsTable.updateEntity(waypoint);
        }
        else
        {
            m_waypointsTable.insertEntity(waypoint);
            m_waypoints.insert(waypoint->id(), waypoint);
        }

        if (!m_routeWaypoints.contains(route, waypoint))
        {
            m_routeWaypoints.insert(route, waypoint);

            m_routeWaypointsTable.insert(
                { { params::route, route->id() }, { params::waypoint, waypoint->id() } });
        }
    }
}

Route* RoutesRepositorySql::readRoute(const QVariant& id)
{
    QVariantMap map = m_routesTable.selectById(id);
    QString typeName = map.value(params::type).toString();

    const RouteType* const type = m_routeTypes.value(typeName);
    if (!type)
    {
        qWarning() << "Unknown route type" << typeName;
        return nullptr;
    }

    Route* route = new Route(map, type, this);
    m_routes.insert(id, route);

    // Read waypoints for route
    for (const QVariantMap& select :
         m_routeWaypointsTable.select({ { params::route, route->id() } }, { params::waypoint }))
    {
        if (select.isEmpty())
            continue;

        const QVariant& waypointId = select.first();
        Waypoint* waypoint = nullptr;
        if (m_waypoints.contains(waypointId))
        {
            waypoint = m_waypoints.value(waypointId);
        }
        {
            waypoint = this->readWaypoint(waypointId);
        }

        route->addWaypoint(waypoint);
        m_routeWaypoints.insert(route, waypoint);
    }

    emit routeAdded(route);
    return route;
}

Waypoint* RoutesRepositorySql::readWaypoint(const QVariant& id)
{
    QVariantMap map = m_waypointsTable.selectById(id);
    QString typeName = map.value(params::type).toString();

    const WaypointType* const type = m_waypointTypes.value(typeName);
    if (!type)
    {
        qWarning() << "Unknown waypoint type" << typeName;
        return nullptr;
    }

    Waypoint* waypoint = new Waypoint(map, type);
    m_waypoints.insert(id, waypoint);
    return waypoint;
}

void RoutesRepositorySql::removeWaypoint(Waypoint* waypoint)
{
    m_waypointsTable.removeEntity(waypoint);
    m_waypoints.remove(waypoint->id());
}
