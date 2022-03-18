#ifndef I_MISSION_ITEMS_REPOSITORY_H
#define I_MISSION_ITEMS_REPOSITORY_H

#include "mission_route_item.h"

namespace md::domain
{
class IMissionItemsRepository
{
public:
    IMissionItemsRepository() = default;
    virtual ~IMissionItemsRepository() = default;

    virtual QVariantMap select(const QVariant& itemId) = 0;
    virtual QVariantList selectMissionItemsIds(const QVariant& missionId) = 0;

    virtual void insert(MissionRouteItem* item, const QVariant& missionId) = 0;
    virtual void read(MissionRouteItem* item) = 0;
    virtual void update(MissionRouteItem* item) = 0;
    virtual void remove(MissionRouteItem* item) = 0;
    virtual void removeById(const QVariant& id) = 0;
};
} // namespace md::domain

#endif // I_MISSION_ITEMS_REPOSITORY_H
