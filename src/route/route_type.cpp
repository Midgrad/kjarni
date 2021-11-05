#include "route_type.h"

using namespace md::domain;

namespace
{
QMap<QString, const WaypointType*> typesToMap(const QVector<const WaypointType*>& waypointTypes)
{
    QMap<QString, const WaypointType*> map;
    for (const WaypointType* type : waypointTypes)
    {
        map[type->name] = type;
    }
    return map;
}
} // namespace

RouteType::RouteType(const QString& name, const QVector<const WaypointType*>& waypointTypes) :
    name(name),
    waypointTypes(::typesToMap(waypointTypes))
{
}

const WaypointType* RouteType::waypointType(const QString& name) const
{
    return this->waypointTypes.value(name, nullptr);
}
