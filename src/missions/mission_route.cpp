#include "mission_route.h"

#include "mission_traits.h"

using namespace md::domain;

MissionRoute::MissionRoute(const MissionType* type, const QVariant& id, const QString& name,
                           QObject* parent) :
    Entity(id, parent),
    m_homePoint(new RouteItem(type->homePointType)),
    m_route(new Route(type->routeType, name))
{
}

MissionRoute::MissionRoute(const MissionType* type, const QVariantMap& map, QObject* parent) :
    Entity(map, parent),
    m_homePoint(new RouteItem(type->homePointType, map.value(props::home).toMap()))
{
}

QVariantMap MissionRoute::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::home, m_homePoint->id());

    if (m_route)
    {
        map.insert(props::route, m_route->id());
    }
    return map;
}

RouteItem* MissionRoute::homePoint() const
{
    return m_homePoint;
}

Route* MissionRoute::route() const
{
    return m_route;
}

int MissionRoute::currentItem() const
{
    return m_currentItem;
}

int MissionRoute::count()
{
    return m_route ? m_route->count() + 1 : 1; // route items (wpt + payload) + home point
}

RouteItem* MissionRoute::item(int index) const
{
    if (index == 0)
        return m_homePoint;

    return m_route ? m_route->item(index - 1) : nullptr;
}

QList<RouteItem*> MissionRoute::items() const
{
    QList<RouteItem*> items;
    items.append(m_homePoint);

    if (m_route)
        items.append(m_route->items());

    return items;
}

void MissionRoute::assignRoute(Route* route)
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
        connect(m_route, &Route::itemAdded, this, &MissionRoute::itemsChanged);
        connect(m_route, &Route::itemChanged, this, &MissionRoute::itemsChanged);
        connect(m_route, &Route::itemRemoved, this, &MissionRoute::itemsChanged);
    }

    emit routeChanged(m_route);
    emit itemsChanged();
}

void MissionRoute::setCurrentItem(int currentItem)
{
    if (m_currentItem == currentItem)
        return;

    m_currentItem = currentItem;
    emit currentItemChanged(m_currentItem);
}
