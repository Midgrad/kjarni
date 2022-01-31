#include "route_item.h"

#include <QDebug>

#include "route_traits.h"

using namespace md::domain;

RouteItem::RouteItem(const RouteItemType* type, const QString& name, const QVariant& id,
                     const QVariantMap& params, const Geodetic& position, const QVariantMap& calcData,
                     bool current, bool reached, QObject* parent) :
    TypedParametrisedMixin<NamedMixin<Entity>>(type->parameters.values().toVector().params, name, id,
                                               parent),
    position(position, std::bind(&Entity::changed, this)),
    calcData(calcData, std::bind(&Entity::changed, this)),
    current(current, std::bind(&Entity::changed, this)),
    reached(reached, std::bind(&Entity::changed, this)),
    m_type(type)
{
    Q_ASSERT(type);
}

RouteItem::RouteItem(const RouteItemType* type, const QVariantMap& map, QObject* parent) :
    RouteItem(type, map.value(props::name).toString(), map.value(props::id),
              map.value(props::params).toMap(), map.value(props::position).toMap(),
              map.value(props::calcData).toMap(), map.value(props::current).toBool(),
              map.value(props::reached).toBool(), parent)
{
}

QVariantMap RouteItem::toVariantMap() const
{
    QVariantMap map = TypedParametrisedMixin<NamedMixin<Entity>>::toVariantMap();

    map.insert(props::type, m_type->id);

    map.insert(props::position, this->position().toVariantMap());
    map.insert(props::calcData, this->calcData());
    map.insert(props::current, this->current());
    map.insert(props::reached, this->reached());

    return map;
}

void RouteItem::fromVariantMap(const QVariantMap& map)
{
    position = map.value(props::position, this->position().toVariantMap()).toMap();
    calcData = map.value(props::calcData, this->calcData()).toMap();
    current = map.value(props::current, this->current()).toBool();
    reached = map.value(props::reached, this->reached()).toBool();

    TypedParametrisedMixin<NamedMixin<Entity>>::fromVariantMap(map);
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

    name = type->shortName;
    this->syncParameters();
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
    for (const ParameterType* parameter : m_type->parameters)
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
