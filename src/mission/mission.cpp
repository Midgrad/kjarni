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
    m_homePoint(new Waypoint(type->homePointType, type->homePointType->name))
{
}

Mission::Mission(const MissionType* type, const QVariantMap& map, QObject* parent) :
    Entity(map, parent),
    m_type(type),
    m_vehicleId(map.value(params::vehicle)),
    m_operation(new MissionOperation(this)),
    m_homePoint(new Waypoint(type->homePointType, map.value(params::home).toMap()))
{
}

QVariantMap Mission::toVariantMap(bool recursive) const
{
    QVariantMap map = Entity::toVariantMap();

    map.insert(params::type, m_type->name);
    map.insert(params::vehicle, m_vehicleId);

    if (recursive)
        map.insert(params::home, m_homePoint->toVariantMap(recursive));
    else
        map.insert(params::home, m_homePoint->id());

    if (m_route)
    {
        if (recursive)
            map.insert(params::route, m_route->toVariantMap(recursive));
        else
            map.insert(params::route, m_route->id());
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
    return m_route ? m_route->count() : 1; // route + home point
}

Waypoint* Mission::waypoint(int index) const
{
    if (index == 0)
        return m_homePoint;
    return m_route->waypoint(index - 1);
}

QList<Waypoint*> Mission::waypoints() const
{
    QList<Waypoint*> waypoints;

    waypoints.append(m_homePoint);
    if (m_route)
        m_route->waypoints();

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

    m_route = route;
    emit routeChanged(m_route);
}
