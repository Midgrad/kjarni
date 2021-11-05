#include "mission_type.h"

using namespace md::domain;

MissionType::MissionType(const QString& name, const RouteType* routeType,
                         const WaypointType* homePointType) :
    name(name),
    routeType(routeType),
    homePointType(homePointType)
{
    Q_ASSERT(routeType);
}
