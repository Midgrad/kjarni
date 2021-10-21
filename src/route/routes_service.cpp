#include "routes_service.h"

#include <QDebug>

#include "utils.h"

namespace
{
constexpr char routes[] = "routes";
constexpr char waypoints[] = "waypoints";
} // namespace

using namespace md::domain;

RoutesService::RoutesService(IRepositoryFactory* repoFactory, QObject* parent) :
    IRoutesService(parent),
    m_routesRepo(repoFactory->create(::routes)),
    m_waypointsRepo(repoFactory->create(::waypoints))
{
}

RoutesService::~RoutesService()
{
}

Route* RoutesService::route(const QVariant& id) const
{
    QMutexLocker locker(&m_mutex);

    return m_routes.value(id, nullptr);
}

QVariantList RoutesService::routeIds() const
{
    QMutexLocker locker(&m_mutex);
    return m_routes.keys();
}

QList<Route*> RoutesService::routes() const
{
    QMutexLocker locker(&m_mutex);
    return m_routes.values();
}

QList<const RouteType*> RoutesService::routeTypes() const
{
    QMutexLocker locker(&m_mutex);
    return m_routeTypes.values();
}

void RoutesService::registerRouteType(const RouteType* routeType)
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

void RoutesService::unregisterRouteType(const RouteType* routeType)
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

void RoutesService::readAll()
{
    QMutexLocker locker(&m_mutex);

    for (const QVariant& routeId : m_routesRepo->selectIds())
    {
        if (!m_routes.contains(routeId))
        {
            this->readRoute(routeId);
        }
    }
}

void RoutesService::removeRoute(Route* route)
{
    QMutexLocker locker(&m_mutex);

    if (route->id().isNull())
        return;

    // Remove stored waypoints for route, new points will be deleted by parent
    for (Waypoint* waypoint : m_routeWaypoints.values(route))
    {
        this->removeWaypoint(waypoint);
    }
    m_routeWaypoints.remove(route);

    // Remove route
    m_routesRepo->remove(route);
    m_routes.remove(route->id());

    emit routeRemoved(route);
    route->deleteLater();
}

void RoutesService::restoreRoute(Route* route)
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

    m_routesRepo->read(route);
    emit routeChanged(route);
}

void RoutesService::saveRoute(Route* route)
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
        m_routesRepo->update(route);
        emit routeChanged(route);
    }
    else
    {
        m_routesRepo->insert(route);
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

        this->removeWaypoint(waypoint);
        m_routeWaypoints.remove(route, waypoint);
    }

    //  Update or insert waypoints
    for (Waypoint* waypoint : route->waypoints())
    {
        if (!m_routeWaypoints.contains(route, waypoint))
        {
            m_routeWaypoints.insert(route, waypoint);
        }

        if (m_waypoints.contains(waypoint->id()))
        {
            m_waypointsRepo->update(waypoint);
        }
        else
        {
            m_waypointsRepo->insert(waypoint);
            m_waypoints.insert(waypoint->id(), waypoint);
        }
    }
}

Route* RoutesService::readRoute(const QVariant& id)
{
    QVariantMap map = m_routesRepo->select(id);
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
    for (const QVariant& waypointId : m_waypointsRepo->selectIds({ { params::route, route->id() } }))
    {
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

Waypoint* RoutesService::readWaypoint(const QVariant& id)
{
    QVariantMap map = m_waypointsRepo->select(id);
    QString typeName = map.value(params::type).toString();

    const WaypointType* const type = m_waypointTypes.value(typeName);
    if (!type)
    {
        qWarning() << "Unknown waypoint type" << typeName;
        return nullptr;
    }

    Waypoint* waypoint = new Waypoint(map, type, this);
    m_waypoints.insert(id, waypoint);
    return waypoint;
}

void RoutesService::removeWaypoint(Waypoint* waypoint)
{
    m_waypointsRepo->remove(waypoint);
    m_waypoints.remove(waypoint->id());
}
