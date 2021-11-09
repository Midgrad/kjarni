#include "route_item.h"

#include <QDebug>

#include "route_traits.h"

using namespace md::domain;

RouteItem::RouteItem(const RouteItemType* type, const QString& name, const QVariant& id,
                     const QVariantMap& parameters, QObject* parent) :
    Entity(id, name, parameters, parent),
    m_type(type)
{
    Q_ASSERT(type);
}

RouteItem::RouteItem(const RouteItemType* type, const QString& name, const QVariant& id,
                     QObject* parent) :
    RouteItem(type, name, id, type->defaultParameters(), parent)
{
}

RouteItem::RouteItem(const RouteItemType* type, const QVariantMap& map, QObject* parent) :
    Entity(map, parent),
    m_type(type),
    m_current(map.value(params::current, false).toBool()),
    m_reached(map.value(params::reached, false).toBool()),
    m_confirmed(map.value(params::confirmed, false).toBool())
{
    Q_ASSERT(type);
}

QVariantMap RouteItem::toVariantMap() const
{
    QVariantMap map = Entity::toVariantMap();

    map.insert(params::type, m_type->id);
    map.insert(params::current, m_current);
    map.insert(params::reached, m_reached);
    map.insert(params::confirmed, m_confirmed);

    return map;
}

void RouteItem::fromVariantMap(const QVariantMap& map)
{
    m_current = map.value(params::current, m_current).toBool();
    m_reached = map.value(params::reached, m_reached).toBool();
    m_confirmed = map.value(params::confirmed, m_confirmed).toBool();

    Entity::fromVariantMap(map);
}

const RouteItemType* RouteItem::type() const
{
    return m_type;
}

bool RouteItem::current() const
{
    return m_current;
}

bool RouteItem::reached() const
{
    return m_reached;
}

bool RouteItem::confirmed() const
{
    return m_confirmed;
}

void RouteItem::setType(const RouteItemType* type)
{
    Q_ASSERT(type);

    if (m_type == type)
        return;

    m_type = type;
    emit changed();

    this->syncParameters();
}

void RouteItem::setCurrent(bool current)
{
    if (m_current == current)
        return;

    m_current = current;
    emit changed();
}

void RouteItem::setConfirmed(bool confirmed)
{
    if (m_confirmed == confirmed)
        return;

    m_confirmed = confirmed;
    emit changed();
}

void RouteItem::setReached(bool reached)
{
    if (m_reached == reached)
        return;

    m_reached = reached;
    emit changed();
}

void RouteItem::setAndCheckParameter(const QString& key, const QVariant& value)
{
    QVariant guarded = value;
    auto parameter = m_type->parameter(key);
    if (parameter)
    {
        guarded = parameter->guard(value);
    }
    this->setParameter(key, guarded);
}

void RouteItem::resetParameter(const QString& key)
{
    auto parameter = m_type->parameter(key);
    if (!parameter)
        return;

    this->setParameter(key, parameter->defaultValue);
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
    for (const QString& key : parameters.keys())
    {
        if (!m_type->parameter(key))
            parameters.remove(key);
    }

    this->setParameters(parameters);
}
