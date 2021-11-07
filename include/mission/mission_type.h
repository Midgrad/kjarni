#ifndef MISSION_TYPE_H
#define MISSION_TYPE_H

#include "route_type.h"

namespace md::domain
{
class MissionType
{
    Q_GADGET

public:
    MissionType(const QString& id, const QString& name, const RouteType* routeType,
                const WaypointType* homePointType);

    const QString id;
    const QString name;
    const RouteType* routeType;
    const WaypointType* homePointType;
};

} // namespace md::domain

#endif // MISSION_TYPE_H
