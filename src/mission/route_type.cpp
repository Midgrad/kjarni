#include "route_type.h"

using namespace md::domain;

RouteType::RouteType(const QString& name, const QVector<WaypointType>& waypointTypes) :
    name(name),
    waypointTypes(waypointTypes)
{
}
