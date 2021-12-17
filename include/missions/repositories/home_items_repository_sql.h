#ifndef HOME_ITEMS_REPOSITORY_SQL_H
#define HOME_ITEMS_REPOSITORY_SQL_H

#include "entity_sql_table.h"
#include "i_home_items_repository.h"

namespace md::data_source
{
class HomeItemsRepositorySql : public domain::IHomeItemsRepository
{
public:
    HomeItemsRepositorySql(QSqlDatabase* database);

    QVariantMap select(const QVariant& itemId) override;
    QVariant selectMissionItemId(const QVariant& missionId) override;

    void insert(domain::RouteItem* item, const QVariant& missionId) override;
    void read(domain::RouteItem* item) override;
    void update(domain::RouteItem* item) override;
    void remove(domain::RouteItem* item) override;
    void removeById(const QVariant& id) override;

private:
    EntitySqlTable m_homeItemsTable;
};
} // namespace md::data_source

#endif // HOME_ITEMS_REPOSITORY_SQL_H
