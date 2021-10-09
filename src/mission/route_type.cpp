#include "route_type.h"

using namespace md::domain;

RouteType::RouteType(const QString& name, const QVector<const WaypointType*>& waypointTypes) :
    name(name),
    waypointTypes(waypointTypes)
{
}

const WaypointType* RouteType::waypointType(const QString& name) const
{
    // TODO: change to map
    auto result = std::find_if(waypointTypes.begin(), waypointTypes.end(),
                               [name](const WaypointType* type) {
                                   return type->name == name;
                               });
    if (result != std::end(waypointTypes))
        return *result;

    return nullptr;
}
