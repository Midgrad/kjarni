#include "route_item.h"

#include <QDebug>

#include "route_traits.h"

using namespace md::domain;

RouteItem::RouteItem(const RouteItemType* type, const QVariant& id, QObject* parent) :
    Entity(id, type->shortName, type->defaultParameters(), parent),
    m_type(type)
{
}

RouteItem::RouteItem(const RouteItemType* type, const QVariantMap& map, QObject* parent) :
    Entity(map, parent),
    m_type(type)
{
    Q_ASSERT(type);
}

QVariantMap RouteItem::toVariantMap() const
{
    QVariantMap map = Entity::toVariantMap();

    map.insert(props::type, m_type->id);

    return map;
}

void RouteItem::fromVariantMap(const QVariantMap& map)
{
    Entity::fromVariantMap(map);
}

const RouteItemType* RouteItem::type() const
{
    return m_type;
}

void RouteItem::setType(const RouteItemType* type)
{
    Q_ASSERT(type);

    if (m_type == type)
        return;

    m_type = type;

    this->setName(type->shortName);
    this->syncParameters();
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
