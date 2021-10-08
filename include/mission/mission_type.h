#ifndef MISSION_TYPE_H
#define MISSION_TYPE_H

#include "route_type.h"

namespace md::domain
{
class MissionType
{
    Q_GADGET

public:
    MissionType(const QString& name, const RouteType& routeType);

    const QString name;
    const RouteType routeType;
};

} // namespace md::domain

#endif // MISSION_TYPE_H
