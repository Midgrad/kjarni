#ifndef I_ROUTE_ITEMS_REPOSITORY_H
#define I_ROUTE_ITEMS_REPOSITORY_H

#include "route_item.h"

namespace md::domain
{
class IRouteItemsRepository
{
public:
    IRouteItemsRepository() = default;
    virtual ~IRouteItemsRepository() = default;

    virtual QVariantMap select(const QVariant& itemId) = 0;
    virtual QVariantList selectChildItemsIds(const QVariant& parentId) = 0;

    virtual void insert(RouteItem* item, const QVariant& parentId) = 0;
    virtual void read(RouteItem* item) = 0;
    virtual void update(RouteItem* item, const QVariant& parentId) = 0;
    virtual void remove(RouteItem* item) = 0;
    virtual void removeById(const QVariant& id) = 0;
};
} // namespace md::domain

#endif // I_ROUTE_ITEMS_REPOSITORY_H
