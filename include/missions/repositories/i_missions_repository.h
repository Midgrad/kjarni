#ifndef I_MISSIONS_REPOSITORY_H
#define I_MISSIONS_REPOSITORY_H

#include "mission.h"

namespace md::domain
{
class IMissionsRepository
{
public:
    IMissionsRepository() = default;
    virtual ~IMissionsRepository() = default;

    virtual QVariantMap select(const QVariant& missionId) = 0;
    virtual QVariantList selectMissionIds() = 0;
    virtual QVariant selectMissionIdForVehicle(const QVariant& vehicleId) = 0;

    virtual void insert(Mission* mission) = 0;
    virtual void read(Mission* mission) = 0;
    virtual void update(Mission* mission) = 0;
    virtual void remove(Mission* mission) = 0;
};
} // namespace md::domain

#endif // I_MISSIONS_REPOSITORY_H
