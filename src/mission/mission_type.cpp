#include "mission_type.h"

using namespace md::domain;

MissionType::MissionType(const QString& id, const QString& name, const RouteType* routeType,
                         const RouteItemType* homePointType) :
    id(id),
    name(name),
    routeType(routeType),
    homePointType(homePointType)
{
    Q_ASSERT(routeType);
}
