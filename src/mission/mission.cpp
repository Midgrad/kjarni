#include "mission.h"

#include <QDebug>

#include "mission_traits.h"

using namespace md::domain;

Mission::Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
                 const QVariant& id, QObject* parent) :
    Entity(id, name, QVariantMap(), parent),
    m_type(type),
    m_vehicleId(vehicleId),
    m_operation(new MissionOperation(this))
{
}

Mission::Mission(const MissionType* type, const QVariantMap& map, QObject* parent) :
    Entity(map, parent),
    m_type(type),
    m_vehicleId(map.value(params::vehicle)),
    m_operation(new MissionOperation(this))
{
}

QVariantMap Mission::toVariantMap(bool recursive) const
{
    QVariantMap map = Entity::toVariantMap();

    map.insert(params::type, m_type->name);
    map.insert(params::vehicle, m_vehicleId);

    if (m_route)
    {
        if (recursive)
            map.insert(params::route, m_route->route()->toVariantMap(recursive));
        else
            map.insert(params::route, m_route->route()->id());
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

MissionRoute* Mission::route() const
{
    return m_route;
}

MissionOperation* Mission::operation() const
{
    return m_operation;
}

void Mission::assignRoute(Route* route)
{
    if (m_route && m_route->route() == route)
        return;

    if (m_route)
        m_route->deleteLater();

    m_route = route ? new MissionRoute(route, this) : nullptr;
    emit routeChanged(m_route);
}
