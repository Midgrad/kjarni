#include "route_type.h"

using namespace md::domain;

namespace
{
QMap<QString, const RouteItemType*> typesToMap(const QVector<const RouteItemType*>& waypointTypes)
{
    QMap<QString, const RouteItemType*> map;
    for (const RouteItemType* type : waypointTypes)
    {
        map[type->id] = type;
    }
    return map;
}
} // namespace

RouteType::RouteType(const QString& id, const QString& name,
                     const QVector<const RouteItemType*>& waypointTypes) :
    id(id),
    name(name),
    waypointTypes(::typesToMap(waypointTypes))
{
}

const RouteItemType* RouteType::waypointType(const QString& id) const
{
    return this->waypointTypes.value(id, nullptr);
}
