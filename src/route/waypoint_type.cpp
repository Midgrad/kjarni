#include "waypoint_type.h"

#include "route_traits.h"
#include "utils.h"

using namespace md::domain;

WaypointType::WaypointType(const QString& id, const QString& name, const QString& shortName,
                           const QVector<const Parameter*>& parameters,
                           const QVector<const RouteItemType*>& itemTypes) :
    RouteItemType(id, name, shortName, parameters),
    itemTypes(utils::listToMap<RouteItemType>(itemTypes))
{
}

const RouteItemType* WaypointType::itemType(const QString& id) const
{
    return itemTypes.value(id, nullptr);
}
