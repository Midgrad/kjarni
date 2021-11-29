#include "mission_route_item.h"

#include "mission_traits.h"

using namespace md::domain;

MissionRouteItem::MissionRouteItem(RouteItem* underlyingItem, QObject* parent) :
    Entity(underlyingItem->id(), parent),
    m_underlyingItem(underlyingItem),
    m_current(false),
    m_reached(false),
    m_confirmed(false)
{
}

QVariantMap MissionRouteItem::toVariantMap() const
{
    QVariantMap map = m_underlyingItem->toVariantMap();

    map[props::current] = m_current;
    map[props::reached] = m_reached;
    map[props::confirmed] = m_confirmed;

    return map;
}

void MissionRouteItem::fromVariantMap(const QVariantMap& map)
{
    m_current = map.value(props::current, m_current).toBool();
    m_reached = map.value(props::reached, m_reached).toBool();
    m_confirmed = map.value(props::confirmed, m_confirmed).toBool();

    m_underlyingItem->fromVariantMap(map);
}

RouteItem* MissionRouteItem::underlyingItem() const
{
    return m_underlyingItem;
}

bool MissionRouteItem::current() const
{
    return m_current;
}

bool MissionRouteItem::reached() const
{
    return m_reached;
}

bool MissionRouteItem::confirmed() const
{
    return m_confirmed;
}

void MissionRouteItem::setCurrent(bool current)
{
    if (m_current == current)
        return;

    m_current = current;
    emit changed();
}

void MissionRouteItem::setReached(bool reached)
{
    if (m_reached == reached)
        return;

    m_reached = reached;
    emit changed();
}

void MissionRouteItem::setConfirmed(bool confirmed)
{
    if (m_confirmed == confirmed)
        return;

    m_confirmed = confirmed;
    emit changed();
}
