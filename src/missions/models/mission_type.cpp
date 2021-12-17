#include "mission_type.h"

using namespace md::domain;

MissionType::MissionType(const QString& id, const QString& name, const RouteType* routeType,
                         const RouteItemType* homePointType, const RouteItemType* targetPointType) :
    id(id),
    name(name),
    routeType(routeType),
    homePointType(homePointType),
    targetPointType(targetPointType)
{
    Q_ASSERT(routeType);
    Q_ASSERT(homePointType);
    Q_ASSERT(targetPointType);
}
