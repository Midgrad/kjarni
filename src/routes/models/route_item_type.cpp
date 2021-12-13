#include "route_item_type.h"

#include "route_item.h"
#include "route_traits.h"
#include "utils.h"

using namespace md::domain;

RouteItemType::RouteItemType(const QString& id, const QString& name, const QString& shortName,
                             const QVector<const Parameter*>& parameters) :
    id(id),
    name(name),
    shortName(shortName),
    parameters(utils::listToMap<Parameter>(parameters))
{
}

QVariantMap RouteItemType::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, id);
    map.insert(props::name, name);
    return map;
}

const Parameter* RouteItemType::parameter(const QString& id) const
{
    return this->parameters.value(id, nullptr);
}

QVariantMap RouteItemType::defaultParameters() const
{
    QVariantMap map;
    for (const Parameter* parameter : parameters)
    {
        map.insert(parameter->id, parameter->defaultValue);
    }
    return map;
}
