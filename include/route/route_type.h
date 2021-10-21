#ifndef ROUTE_TYPE_H
#define ROUTE_TYPE_H

#include "waypoint_type.h"

namespace md::domain
{
class RouteType
{
    Q_GADGET

public:
    RouteType(const QString& name, const QVector<const WaypointType*>& waypointTypes);

    const WaypointType* waypointType(const QString& name) const;

    const QString name;
    const QVector<const WaypointType*> waypointTypes;
};
} // namespace md::domain

#endif // ROUTE_TYPE_H
