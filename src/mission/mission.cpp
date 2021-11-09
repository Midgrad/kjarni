#include "mission.h"

#include <QDebug>

#include "mission_traits.h"

using namespace md::domain;

Mission::Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
                 const QVariant& id, QObject* parent) :
    Entity(id, name, QVariantMap(), parent),
    m_type(type),
    m_vehicleId(vehicleId),
    m_operation(new MissionOperation(this)),
    m_homePoint(new Waypoint(type->homePointType, type->homePointType->shortName))
{
}

Mission::Mission(const MissionType* type, const QVariantMap& map, QObject* parent) :
    Entity(map, parent),
    m_type(type),
    m_vehicleId(map.value(props::vehicle)),
    m_operation(new MissionOperation(this)),
    m_homePoint(new Waypoint(type->homePointType, map.value(props::home).toMap()))
{
}

QVariantMap Mission::toVariantMap() const
{
    QVariantMap map = Entity::toVariantMap();

    map.insert(props::type, m_type->id);
    map.insert(props::vehicle, m_vehicleId);
    map.insert(props::home, m_homePoint->id());

    if (m_route)
    {
        map.insert(props::route, m_route->id());
    }

    return map;
}

const MissionType* Mission::type() const
{
    return m_type;
}

QVariant Mission::vehicleId() const
{
    return m_vehicleId;
}

Route* Mission::route() const
{
    return m_route;
}

int Mission::count() const
{
    return m_route ? m_route->waypointsCount() : 1; // route + home point
}

Waypoint* Mission::waypoint(int index) const
{
    if (index == 0)
        return m_homePoint;

    return m_route ? m_route->waypoint(index - 1) : nullptr;
}

Waypoint* Mission::currentWaypoint() const
{
    return m_currentWaypoint;
}

int Mission::currentWaypointIndex() const
{
    return this->waypoints().indexOf(m_currentWaypoint);
}

QList<Waypoint*> Mission::waypoints() const
{
    QList<Waypoint*> waypoints;

    waypoints.append(m_homePoint);
    if (m_route)
        waypoints += m_route->waypoints();

    return waypoints;
}

MissionOperation* Mission::operation() const
{
    return m_operation;
}

Waypoint* Mission::homePoint() const
{
    return m_homePoint;
}

void Mission::assignRoute(Route* route)
{
    if (m_route == route)
        return;

    if (m_route)
    {
        disconnect(m_route, nullptr, this, nullptr);
    }

    m_route = route;

    if (m_route)
    {
        connect(m_route, &Route::waypointAdded, this, &Mission::waypointsChanged);
        connect(m_route, &Route::waypointChanged, this, &Mission::waypointsChanged);
        connect(m_route, &Route::waypointRemoved, this, &Mission::waypointsChanged);
    }

    emit routeChanged(m_route);
    emit waypointsChanged();
}

void Mission::setCurrentWaypointIndex(int currentWaypointIndex)
{
    Waypoint* currentWaypoint = this->waypoint(currentWaypointIndex);

    if (m_currentWaypoint == currentWaypoint)
        return;

    if (m_currentWaypoint)
        m_currentWaypoint->setCurrent(false);

    m_currentWaypoint = currentWaypoint;

    if (m_currentWaypoint)
        m_currentWaypoint->setCurrent(true);

    emit currentWaypointChanged(currentWaypointIndex);
}
