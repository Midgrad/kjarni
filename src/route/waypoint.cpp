#include "waypoint.h"

#include <QDebug>

#include "route_traits.h"
#include "utils.h"

using namespace md::domain;

Waypoint::Waypoint(const WaypointType* type, const QVariant& id, QObject* parent) :
    RouteItem(type, id, parent),
    m_type(type)
{
}

Waypoint::Waypoint(const WaypointType* type, const QVariantMap& map, QObject* parent) :
    RouteItem(type, map, parent),
    m_type(type),
    m_calcData(map.value(props::calcData).toMap()),
    m_position(map),
    m_current(map.value(props::current, false).toBool()),
    m_reached(map.value(props::reached, false).toBool()),
    m_confirmed(map.value(props::confirmed, false).toBool())
{
}

QVariantMap Waypoint::toVariantMap() const
{
    QVariantMap map = RouteItem::toVariantMap();

    map.insert(props::calcData, m_calcData);

    utils::mergeMap(map, m_position.toVariantMap());

    map.insert(props::current, m_current);
    map.insert(props::reached, m_reached);
    map.insert(props::confirmed, m_confirmed);

    return map;
}

void Waypoint::fromVariantMap(const QVariantMap& map)
{
    m_calcData = map.value(props::calcData, m_calcData).toMap();

    m_position = map;

    m_current = map.value(props::current, m_current).toBool();
    m_reached = map.value(props::reached, m_reached).toBool();
    m_confirmed = map.value(props::confirmed, m_confirmed).toBool();

    Entity::fromVariantMap(map);
}

const WaypointType* Waypoint::type() const
{
    return m_type;
}

QVariantMap Waypoint::calcData()
{
    return m_calcData;
}

const Geodetic& Waypoint::position() const
{
    return m_position;
}

int Waypoint::count() const
{
    return m_items.count();
}

int Waypoint::index(RouteItem* item) const
{
    return m_items.indexOf(item);
}

RouteItem* Waypoint::item(int index) const
{
    return m_items.value(index, nullptr);
}

const QList<RouteItem*>& Waypoint::items() const
{
    return m_items;
}

bool Waypoint::current() const
{
    return m_current;
}

bool Waypoint::reached() const
{
    return m_reached;
}

bool Waypoint::confirmed() const
{
    return m_confirmed;
}

void Waypoint::setType(const WaypointType* type)
{
    if (m_type == type)
        return;

    m_type = type;
    RouteItem::setType(type);
}

void Waypoint::setCalcData(const QVariantMap& calcData)
{
    if (m_calcData == calcData)
        return;

    m_calcData = calcData;
    emit changed();
}

void Waypoint::setPosition(const Geodetic& position)
{
    if (m_position == position)
        return;

    m_position = position;
    emit changed();
}

void Waypoint::setItems(const QList<RouteItem*>& items)
{
    // Remove old items (std::remove_if does not emit signals)
    for (RouteItem* item : qAsConst(m_items))
    {
        // Skip item if we have it in new list
        if (items.contains(item))
            continue;

        this->removeItem(item);
    }

    // Add new items to the end
    for (RouteItem* item : items)
    {
        this->addItem(item);
    }
}

void Waypoint::addItem(RouteItem* item)
{
    if (m_items.contains(item))
        return;

    if (item->thread() != this->thread())
        item->moveToThread(this->thread());

    if (!item->parent())
        item->setParent(this);

    connect(item, &RouteItem::changed, this, [item, this]() {
        emit itemChanged(this->index(item), item);
        emit changed();
    });

    m_items.append(item);
    emit itemAdded(m_items.count() - 1, item);
    emit changed();
}

void Waypoint::removeItem(RouteItem* item)
{
    int index = m_items.indexOf(item);
    // Remove but don't delete item
    if (index == -1)
        return;

    if (item->parent() == this)
        item->setParent(nullptr);

    disconnect(item, nullptr, this, nullptr);

    m_items.removeOne(item);
    emit itemRemoved(index, item);
    emit changed();
}

void Waypoint::setCurrent(bool current)
{
    if (m_current == current)
        return;

    m_current = current;
    emit changed();
}

void Waypoint::setConfirmed(bool confirmed)
{
    if (m_confirmed == confirmed)
        return;

    m_confirmed = confirmed;
    emit changed();
}

void Waypoint::setReached(bool reached)
{
    if (m_reached == reached)
        return;

    m_reached = reached;
    emit changed();
}
