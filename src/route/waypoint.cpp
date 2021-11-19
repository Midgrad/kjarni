#include "waypoint.h"

#include <QDebug>

#include "route_traits.h"
#include "utils.h"

using namespace md::domain;

Waypoint::Waypoint(const WaypointType* type, const QVariant& id, QObject* parent) :
    WaypointItem(type, id, parent),
    m_type(type)
{
}

Waypoint::Waypoint(const WaypointType* type, const QVariantMap& map, QObject* parent) :
    WaypointItem(type, map, parent),
    m_type(type),
    m_calcData(map.value(props::calcData).toMap()),
    m_position(map)
{
}

QVariantMap Waypoint::toVariantMap() const
{
    QVariantMap map = WaypointItem::toVariantMap();

    map.insert(props::calcData, m_calcData);
    utils::mergeMap(map, m_position.toVariantMap());

    return map;
}

void Waypoint::fromVariantMap(const QVariantMap& map)
{
    m_calcData = map.value(props::calcData, m_calcData).toMap();
    m_position = map;

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

int Waypoint::index(WaypointItem* item) const
{
    return m_items.indexOf(item);
}

WaypointItem* Waypoint::item(int index) const
{
    return m_items.value(index, nullptr);
}

const QList<WaypointItem*>& Waypoint::items() const
{
    return m_items;
}

void Waypoint::setType(const WaypointType* type)
{
    if (m_type == type)
        return;

    m_type = type;
    WaypointItem::setType(type);

    for (WaypointItem* item : qAsConst(m_items))
    {
        if (type->itemTypes.contains(item->type()->id))
            continue;

        this->removeItem(item);
    }
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

void Waypoint::setItems(const QList<WaypointItem*>& items)
{
    // Remove old items (std::remove_if does not emit signals)
    for (WaypointItem* item : qAsConst(m_items))
    {
        // Skip item if we have it in new list
        if (items.contains(item))
            continue;

        this->removeItem(item);
    }

    // Add new items to the end
    for (WaypointItem* item : items)
    {
        this->addItem(item);
    }
}

void Waypoint::addItem(WaypointItem* item)
{
    if (m_items.contains(item))
        return;

    if (item->thread() != this->thread())
        item->moveToThread(this->thread());

    if (!item->parent())
        item->setParent(this);

    connect(item, &WaypointItem::changed, this, [item, this]() {
        emit itemChanged(this->index(item), item);
        emit changed();
    });

    m_items.append(item);
    emit itemAdded(m_items.count() - 1, item);
    emit changed();
}

void Waypoint::removeItem(WaypointItem* item)
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
