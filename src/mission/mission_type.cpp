#include "mission_type.h"

using namespace md::domain;

MissionType::MissionType(const QString& name, const RouteType& routeType) :
    name(name),
    routeType(routeType)
{
}

bool MissionType::isNull() const
{
    return name.isEmpty();
}
