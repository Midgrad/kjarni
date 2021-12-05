#include "route_item.h"

#include <QDebug>

#include "route_traits.h"

using namespace md::domain;

RouteItem::RouteItem(const RouteItemType* type, const QVariant& id, QObject* parent) :
    Parametrised(id, type->shortName, type->defaultParameters(), parent),
    m_type(type)
{
}

RouteItem::RouteItem(const RouteItemType* type, const QVariantMap& map, QObject* parent) :
    Parametrised(map, parent),
    m_type(type),
    m_calcData(map.value(props::calcData).toMap())
{
    Q_ASSERT(type);
}

QVariantMap RouteItem::toVariantMap() const
{
    QVariantMap map = Parametrised::toVariantMap();
    map.insert(props::type, m_type->id);
    map.insert(props::calcData, m_calcData);
    map.insert(props::current, m_current);
    map.insert(props::reached, m_reached);

    return map;
}

void RouteItem::fromVariantMap(const QVariantMap& map)
{
    m_calcData = map.value(props::calcData, m_calcData).toMap();

    Parametrised::fromVariantMap(map);
}

const RouteItemType* RouteItem::type() const
{
    return m_type;
}

QVariantMap RouteItem::calcData()
{
    return m_calcData;
}

bool RouteItem::isCurrent() const
{
    return m_current;
}

bool RouteItem::isReached() const
{
    return m_reached;
}

int RouteItem::count() const
{
    return m_items.count();
}

int RouteItem::index(RouteItem* item) const
{
    return m_items.indexOf(item);
}

RouteItem* RouteItem::item(int index) const
{
    return m_items.value(index, nullptr);
}

const QList<RouteItem*>& RouteItem::items() const
{
    return m_items;
}

void RouteItem::setType(const RouteItemType* type)
{
    Q_ASSERT(type);

    if (m_type == type)
        return;

    m_type = type;

    this->setName(type->shortName);
    this->syncParameters();

    // Remove untyped children
    for (RouteItem* item : qAsConst(m_items))
    {
        if (type->childTypes.contains(item->type()->id))
            continue;

        this->removeItem(item);
    }
}

void RouteItem::setAndCheckParameter(const QString& paramId, const QVariant& value)
{
    QVariant guarded = value;
    auto parameter = m_type->parameter(paramId);
    if (parameter)
    {
        guarded = parameter->guard(value);
    }
    this->setParameter(paramId, guarded);
}

void RouteItem::resetParameter(const QString& paramId)
{
    auto parameter = m_type->parameter(paramId);
    if (!parameter)
        return;

    this->setParameter(paramId, parameter->defaultValue);
}

void RouteItem::resetParameters()
{
    this->setParameters(m_type->defaultParameters());
}

void RouteItem::syncParameters()
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

void RouteItem::setCalcData(const QVariantMap& calcData)
{
    if (m_calcData == calcData)
        return;

    m_calcData = calcData;
    emit changed();
}

void RouteItem::setCurrent(bool current)
{
    if (m_current == current)
        return;

    m_current = current;
    emit changed();
}

void RouteItem::setReached(bool reached)
{
    if (m_reached == reached)
        return;

    m_reached = reached;
    emit changed();
}

void RouteItem::setItems(const QList<RouteItem*>& items)
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

void RouteItem::addItem(RouteItem* item)
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

void RouteItem::removeItem(RouteItem* item)
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
