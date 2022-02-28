#ifndef ROUTE_ITEMS_REPOSITORY_SQL_H
#define ROUTE_ITEMS_REPOSITORY_SQL_H

#include "entity_sql_table.h"
#include "i_route_items_repository.h"

namespace md::data_source
{
class RouteItemsRepositorySql : public domain::IRouteItemsRepository
{
public:
    explicit RouteItemsRepositorySql(QSqlDatabase* database);

    QVariantMap select(const QVariant& itemId) override;
    QVariantList selectRouteItemsIds(const QVariant& routeId) override;

    void insert(domain::RouteItem* item, const QVariant& routeId) override;
    void read(domain::RouteItem* item) override;
    void update(domain::RouteItem* item) override;
    void remove(domain::RouteItem* item) override;
    void removeById(const QVariant& id) override;

private:
    EntitySqlTable m_routeItemsTable;
};
} // namespace md::data_source

#endif // ROUTE_ITEMS_REPOSITORY_SQL_H
