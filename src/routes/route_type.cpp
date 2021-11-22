#include "route_type.h"

#include "route_traits.h"
#include "utils.h"

using namespace md::domain;

RouteType::RouteType(const QString& id, const QString& name,
                     const QVector<const WaypointItemType*>& waypointTypes) :
    id(id),
    name(name),
    waypointTypes(utils::listToMap<WaypointItemType>(waypointTypes))
{
}

QVariantMap RouteType::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, id);
    map.insert(props::name, name);
    return map;
}

const WaypointItemType* RouteType::waypointType(const QString& id) const
{
    return this->waypointTypes.value(id, nullptr);
}
