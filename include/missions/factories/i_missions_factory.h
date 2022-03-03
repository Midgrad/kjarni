#ifndef I_MISSIONS_FACTORY_H
#define I_MISSIONS_FACTORY_H

#include "mission.h"

namespace md::domain
{
class IMissionsFactory
{
public:
    IMissionsFactory() = default;
    virtual ~IMissionsFactory() = default;

    virtual Mission* createMission(const QVariantMap& map) = 0;
    virtual MissionRouteItem* createMissionRouteItem(const QVariantMap& map) = 0;
};
} // namespace md::domain

#endif // I_MISSIONS_FACTORY_H
