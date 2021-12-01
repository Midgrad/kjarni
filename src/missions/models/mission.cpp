#include "mission.h"

#include <QDebug>

#include "mission_traits.h"

using namespace md::domain;

Mission::Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
                 const QVariant& id, QObject* parent) :
    Named(id, name, parent),
    m_type(type),
    m_route(new MissionRoute(type, id, this)),
    m_vehicleId(vehicleId)
{
}

Mission::Mission(const MissionType* type, const QVariantMap& map, QObject* parent) :
    Named(map, parent),
    m_type(type),
    m_route(new MissionRoute(type, map.value(props::id), this)),
    m_vehicleId(map.value(props::vehicle))
{
}

QVariantMap Mission::toVariantMap() const
{
    QVariantMap map = Named::toVariantMap();

    map.insert(props::type, m_type->id);
    map.insert(props::vehicle, m_vehicleId);
    if (m_route)
        map.insert(props::route, m_route->id());

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

void Mission::clear()
{
    if (m_route)
        m_route->clear();
}
