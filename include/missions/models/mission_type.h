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
                const RouteItemType* homePointType, const RouteItemType* targetPointType);

    const QString id;
    const QString name;
    const RouteType* routeType;
    const RouteItemType* homePointType;
    const RouteItemType* targetPointType;
};

} // namespace md::domain

#endif // MISSION_TYPE_H
