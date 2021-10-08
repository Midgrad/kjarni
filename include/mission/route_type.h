#ifndef ROUTE_TYPE_H
#define ROUTE_TYPE_H

#include "waypoint_type.h"

namespace md::domain
{
class RouteType
{
    Q_GADGET

public:
    RouteType(const QString& name, const QVector<WaypointType>& waypointTypes);

    const QString name;
    const QVector<WaypointType> waypointTypes;
};
} // namespace md::domain

#endif // ROUTE_TYPE_H
