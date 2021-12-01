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

QList<MissionRouteItem*> Mission::items() const
{
    QList<MissionRouteItem*> items;

    items.append(m_homePoint);
    if (m_route)
        items.append(m_route->items());

    return items;
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
    {
        m_route = new MissionRoute(route);
        m_route->moveToThread(this->thread());
        m_route->setParent(this);
    }
    else
    {
        m_route = nullptr;
    }
    emit routeChanged(m_route);
}

void Mission::clear()
{
    if (m_route)
        m_route->clear();
}
