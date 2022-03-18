#include "mission_item_type.h"

#include "mission_route_item.h"
#include "mission_traits.h"
#include "utils.h"

using namespace md::domain;

MissionItemType::MissionItemType(const QString& id, const QString& name, const QString& shortName,
                                 Positioned positioned,
                                 const QVector<const ParameterType*>& parameters) :
    id(id),
    name(name),
    shortName(shortName),
    positioned(positioned),
    parameters(utils::vecToMap<const ParameterType*>(parameters))
{
}

QVariantMap MissionItemType::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, id);
    map.insert(props::name, name);
    map.insert(props::shortName, name);
    return map;
}

const ParameterType* MissionItemType::parameter(const QString& id) const
{
    return this->parameters.value(id, nullptr);
}

QVariantMap MissionItemType::defaultParameters() const
{
    QVariantMap map;
    for (const ParameterType* parameter : parameters)
    {
        map.insert(parameter->id, parameter->defaultValue);
    }
    return map;
}
