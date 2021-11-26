#include "mission.h"

#include <QDebug>

#include "mission_traits.h"

using namespace md::domain;

Mission::Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
                 const QVariant& id, QObject* parent) :
    Named(id, name, parent),
    m_type(type),
    m_vehicleId(vehicleId),
    m_operation(new MissionOperation(this)),
    m_route(new MissionRoute(type, id, name, this))
{
}

Mission::Mission(const MissionType* type, const QVariantMap& map, QObject* parent) :
    Named(map, parent),
    m_type(type),
    m_vehicleId(map.value(props::vehicle)),
    m_operation(new MissionOperation(this)),
    m_route(new MissionRoute(type, map, this))
{
}

QVariantMap Mission::toVariantMap() const
{
    QVariantMap map = Named::toVariantMap();

    map.insert(props::type, m_type->id);
    map.insert(props::vehicle, m_vehicleId);

    utils::mergeMap(map, m_route->toVariantMap());

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

MissionOperation* Mission::operation() const
{
    return m_operation;
}

MissionRoute* Mission::route() const
{
    return m_route;
}
