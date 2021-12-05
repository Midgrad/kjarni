#include "route_item.h"

#include <QDebug>

#include "route_traits.h"

using namespace md::domain;

RouteItem::RouteItem(const RouteItemType* type, const QVariant& id, const QString& name,
                     QVariantMap params, const QVariantMap& calcData, bool current, bool reached,
                     QObject* parent) :
    Parametrised(id, name, params, parent),
    calcData(calcData, std::bind(&Entity::changed, this)),
    current(current, std::bind(&Entity::changed, this)),
    reached(reached, std::bind(&Entity::changed, this)),
    m_type(type)
{
    Q_ASSERT(type);
}

RouteItem::RouteItem(const RouteItemType* type, const QVariant& id, const QVariantMap& calcData,
                     QObject* parent) :
    RouteItem(type, id, type->shortName, calcData, type->defaultParameters(), false, false, parent)
{
}

RouteItem::RouteItem(const RouteItemType* type, const QVariantMap& map, QObject* parent) :
    RouteItem(type, map.value(props::id), map.value(props::name).toString(),
              map.value(props::params).toMap(), map.value(props::calcData).toMap(),
              map.value(props::current).toBool(), map.value(props::reached).toBool(), parent)
{
}

QVariantMap RouteItem::toVariantMap() const
{
    QVariantMap map = Parametrised::toVariantMap();
    map.insert(props::type, m_type->id);
    map.insert(props::calcData, calcData.get());
    map.insert(props::current, current.get());
    map.insert(props::reached, reached.get());

    return map;
}

void RouteItem::fromVariantMap(const QVariantMap& map)
{
    calcData = map.value(props::calcData, calcData.get()).toMap();
    current = map.value(props::current, current.get()).toBool();
    reached = map.value(props::reached, reached.get()).toBool();

    Parametrised::fromVariantMap(map);
}

const RouteItemType* RouteItem::type() const
{
    return m_type;
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

    name = type->shortName;
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
