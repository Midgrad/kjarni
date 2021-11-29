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
    m_homePoint(new MissionRouteItem(new RouteItem(type->homePointType, utils::generateId(), this)))
{
}

Mission::Mission(const MissionType* type, const QVariantMap& map, QObject* parent) :
    Named(map, parent),
    m_type(type),
    m_vehicleId(map.value(props::vehicle)),
    m_operation(new MissionOperation(this)),
    m_homePoint(new MissionRouteItem(
        new RouteItem(type->homePointType, map.value(props::home).toMap(), this)))
{
}

QVariantMap Mission::toVariantMap() const
{
    QVariantMap map = Named::toVariantMap();

    map.insert(props::type, m_type->id);
    map.insert(props::vehicle, m_vehicleId);

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

MissionRouteItem* Mission::homePoint() const
{
    return m_homePoint;
}

MissionRoute* Mission::route() const
{
    return m_route;
}

void Mission::assignRoute(Route* route)
{
    if (m_route)
        m_route->deleteLater();

    if (route)
        m_route = new MissionRoute(route, this);
    else
        m_route = nullptr;

    emit routeChanged();
}
