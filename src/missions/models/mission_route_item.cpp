#include "mission_route_item.h"

#include <QDebug>

#include "route_traits.h"

using namespace md::domain;

MissionRouteItem::MissionRouteItem(const QString& name, const ParameterList& parameters,
                                   const QVariant& id, QObject* parent) :
    NamedMixin<Entity>(name, id, parent),
    parameters(parameters),
    position(Geodetic(), std::bind(&Entity::changed, this)),
    current(false, std::bind(&Entity::changed, this)),
    reached(false, std::bind(&Entity::changed, this))
{
    for (Parameter* parameter : parameters)
    {
        parameter->setParent(this);
    }
}

QVariantMap MissionRouteItem::toVariantMap() const
{
    QVariantMap map = NamedMixin<Entity>::toVariantMap();

    map.insert(props::position, this->position().toVariantMap());
    map.insert(props::current, this->current());
    map.insert(props::reached, this->reached());

    QVariantMap parameterMap;
    for (Parameter* parameter : this->parameters())
    {
        parameterMap.insert(parameter->id().toString(), parameter->value());
    }

    return map;
}

void MissionRouteItem::fromVariantMap(const QVariantMap& map)
{
    position = map.value(props::position, this->position().toVariantMap()).toMap();
    current = map.value(props::current, this->current()).toBool();
    reached = map.value(props::reached, this->reached()).toBool();

    QVariantMap parameterMap = map.value(props::params).toMap();
    for (Parameter* parameter : this->parameters())
    {
        parameter->setAndCheckValue(
            parameterMap.value(parameter->id().toString(), parameter->value));
    }

    NamedMixin<Entity>::fromVariantMap(map);
}
