#include "waypoint_item_type.h"

#include "route_traits.h"
#include "utils.h"
#include "waypoint_item.h"

using namespace md::domain;

WaypointItemType::WaypointItemType(const QString& id, const QString& name, const QString& shortName,
                                   const QVector<const Parameter*>& parameters,
                                   const QVector<const WaypointItemType*>& childTypes) :
    id(id),
    name(name),
    shortName(shortName),
    parameters(utils::listToMap<Parameter>(parameters)),
    childTypes(utils::listToMap<WaypointItemType>(childTypes))
{
}

QVariantMap WaypointItemType::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, id);
    map.insert(props::name, name);
    return map;
}

const Parameter* WaypointItemType::parameter(const QString& id) const
{
    return this->parameters.value(id, nullptr);
}

QVariantMap WaypointItemType::defaultParameters() const
{
    QVariantMap map;
    for (const Parameter* parameter : parameters)
    {
        map.insert(parameter->id, parameter->defaultValue);
    }
    return map;
}

const WaypointItemType* WaypointItemType::childType(const QString& id) const
{
    return childTypes.value(id, nullptr);
}
