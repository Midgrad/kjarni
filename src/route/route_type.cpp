#include "route_type.h"

#include "route_traits.h"
#include "utils.h"

using namespace md::domain;

RouteType::RouteType(const QString& id, const QString& name,
                     const QVector<const WaypointType*>& waypointTypes) :
    id(id),
    name(name),
    waypointTypes(utils::listToMap<WaypointType>(waypointTypes))
{
}

QVariantMap RouteType::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, id);
    map.insert(props::name, name);
    return map;
}

const WaypointType* RouteType::waypointType(const QString& id) const
{
    return this->waypointTypes.value(id, nullptr);
}
