#include "route_pattern.h"

#include "route_traits.h"
#include "utils.h"

using namespace md::domain;

RoutePattern::RoutePattern(const QString& id, const QString& name,
                           const QVector<const Parameter*>& parameters) :
    id(id),
    name(name),
    parameters(utils::listToMap<Parameter>(parameters))
{
}

QVariantMap RoutePattern::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, id);
    map.insert(props::name, name);
    return map;
}

const Parameter* RoutePattern::parameter(const QString& id) const
{
    return this->parameters.value(id, nullptr);
}

QVariantMap RoutePattern::defaultParameters() const
{
    QVariantMap map;
    for (const Parameter* parameter : parameters)
    {
        map.insert(parameter->id, parameter->defaultValue);
    }
    return map;
}
