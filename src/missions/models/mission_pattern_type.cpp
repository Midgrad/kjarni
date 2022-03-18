#include "mission_pattern_type.h"

#include "utils.h"

using namespace md::domain;

RoutePatternType::RoutePatternType(const QString& id, const QString& name, const QString& icon,
                                   const QVector<const ParameterType*>& parameters) :
    id(id),
    name(name),
    icon(icon),
    parameters(utils::vecToMap<const ParameterType*>(parameters))
{
}

QVariantMap RoutePatternType::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, id);
    map.insert(props::name, name);
    map.insert(props::icon, icon);
    return map;
}

const ParameterType* RoutePatternType::parameter(const QString& id) const
{
    return this->parameters.value(id, nullptr);
}

QVariantMap RoutePatternType::defaultParameters() const
{
    QVariantMap map;
    for (const ParameterType* parameter : parameters)
    {
        map.insert(parameter->id, parameter->defaultValue);
    }
    return map;
}
