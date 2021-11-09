#include "route_type.h"

#include "utils.h"

using namespace md::domain;

RouteType::RouteType(const QString& id, const QString& name,
                     const QVector<const WaypointType*>& waypointTypes) :
    id(id),
    name(name),
    waypointTypes(utils::listToMap<WaypointType>(waypointTypes))
{
}

const WaypointType* RouteType::waypointType(const QString& id) const
{
    return this->waypointTypes.value(id, nullptr);
}
