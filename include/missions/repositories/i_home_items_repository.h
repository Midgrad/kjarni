#ifndef I_HOME_ITEMS_REPOSITORY_SQL_H
#define I_HOME_ITEMS_REPOSITORY_SQL_H

#include "route_item.h"

namespace md::domain
{
class IHomeItemsRepository
{
public:
    IHomeItemsRepository() = default;
    virtual ~IHomeItemsRepository() = default;

    virtual QVariantMap select(const QVariant& itemId) = 0;
    virtual QVariant selectMissionItemId(const QVariant& missionId) = 0;

    virtual void insert(RouteItem* item, const QVariant& missionId) = 0;
    virtual void read(RouteItem* item) = 0;
    virtual void update(RouteItem* item) = 0;
    virtual void remove(RouteItem* item) = 0;
    virtual void removeById(const QVariant& id) = 0;
};
} // namespace md::domain

#endif // I_HOME_ITEMS_REPOSITORY_SQL_H
