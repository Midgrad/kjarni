#include "waypoint_item.h"

#include <QDebug>

#include "route_traits.h"

using namespace md::domain;

WaypointItem::WaypointItem(const WaypointItemType* type, const QVariant& id, QObject* parent) :
    Parametrised(id, type->shortName, type->defaultParameters(), parent),
    m_type(type)
{
}

WaypointItem::WaypointItem(const WaypointItemType* type, const QVariantMap& map, QObject* parent) :
    Parametrised(map, parent),
    m_type(type),
    m_calcData(map.value(props::calcData).toMap())
{
    Q_ASSERT(type);
}

QVariantMap WaypointItem::toVariantMap() const
{
    QVariantMap map = Parametrised::toVariantMap();
    map.insert(props::type, m_type->id);
    map.insert(props::calcData, m_calcData);
    return map;
}

void WaypointItem::fromVariantMap(const QVariantMap& map)
{
    m_calcData = map.value(props::calcData, m_calcData).toMap();

    Parametrised::fromVariantMap(map);
}

const WaypointItemType* WaypointItem::type() const
{
    return m_type;
}

QVariantMap WaypointItem::calcData()
{
    return m_calcData;
}

int WaypointItem::count() const
{
    return m_items.count();
}

int WaypointItem::index(WaypointItem* item) const
{
    return m_items.indexOf(item);
}

WaypointItem* WaypointItem::item(int index) const
{
    return m_items.value(index, nullptr);
}

const QList<WaypointItem*>& WaypointItem::items() const
{
    return m_items;
}

void WaypointItem::setType(const WaypointItemType* type)
{
    Q_ASSERT(type);

    if (m_type == type)
        return;

    m_type = type;

    this->setName(type->shortName);
    this->syncParameters();

    // Remove untyped children
    for (WaypointItem* item : qAsConst(m_items))
    {
        if (type->childTypes.contains(item->type()->id))
            continue;

        this->removeItem(item);
    }
}

void WaypointItem::setAndCheckParameter(const QString& paramId, const QVariant& value)
{
    QVariant guarded = value;
    auto parameter = m_type->parameter(paramId);
    if (parameter)
    {
        guarded = parameter->guard(value);
    }
    this->setParameter(paramId, guarded);
}

void WaypointItem::resetParameter(const QString& paramId)
{
    auto parameter = m_type->parameter(paramId);
    if (!parameter)
        return;

    this->setParameter(paramId, parameter->defaultValue);
}

void WaypointItem::resetParameters()
{
    this->setParameters(m_type->defaultParameters());
}

void WaypointItem::syncParameters()
{
    QVariantMap parameters = this->parameters();

    // Add parameters defaulted by type
    for (const Parameter* parameter : m_type->parameters)
    {
        if (!parameters.contains(parameter->id))
            parameters.insert(parameter->id, parameter->defaultValue);
    }

    // Remove unneeded parameters
    for (const QString& paramId : parameters.keys())
    {
        if (!m_type->parameter(paramId))
            parameters.remove(paramId);
    }

    this->setParameters(parameters);
}

void WaypointItem::setCalcData(const QVariantMap& calcData)
{
    if (m_calcData == calcData)
        return;

    m_calcData = calcData;
    emit changed();
}

void WaypointItem::setItems(const QList<WaypointItem*>& items)
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

void WaypointItem::addItem(WaypointItem* item)
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

void WaypointItem::removeItem(WaypointItem* item)
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
