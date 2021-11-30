#include "mission.h"

#include <QDebug>

#include "mission_traits.h"

using namespace md::domain;

Mission::Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
                 const QVariant& id, QObject* parent) :
    Named(id, name, parent),
    m_type(type),
    m_vehicleId(vehicleId),
    m_homePoint(new MissionRouteItem(new RouteItem(type->homePointType, utils::generateId(), this)))
{
}

Mission::Mission(const MissionType* type, const QVariantMap& map, QObject* parent) :
    Named(map, parent),
    m_type(type),
    m_vehicleId(map.value(props::vehicle)),
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

MissionRouteItem* Mission::homePoint() const
{
    return m_homePoint;
}

MissionRouteItem* Mission::item(int index) const
{
    if (index == 0)
        return m_homePoint;

    return m_route ? m_route->item(index - 1) : nullptr;
}

MissionRoute* Mission::route() const
{
    return m_route;
}

int Mission::count() const
{
    int count = 1; // for home point

    if (m_route)
        count += m_route->count();

    return count;
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

void Mission::clear()
{
    if (m_route)
        m_route->clear();
}
