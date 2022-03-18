#ifndef MISSION_ITEMS_REPOSITORY_SQL_H
#define MISSION_ITEMS_REPOSITORY_SQL_H

#include "entity_sql_table.h"
#include "i_mission_items_repository.h"

namespace md::data_source
{
class MissionItemsRepositorySql : public domain::IMissionItemsRepository
{
public:
    explicit MissionItemsRepositorySql(QSqlDatabase* database);

    QVariantMap select(const QVariant& itemId) override;
    QVariantList selectMissionItemsIds(const QVariant& missionId) override;

    void insert(domain::MissionRouteItem* item, const QVariant& missionId) override;
    void read(domain::MissionRouteItem* item) override;
    void update(domain::MissionRouteItem* item) override;
    void remove(domain::MissionRouteItem* item) override;
    void removeById(const QVariant& id) override;

private:
    EntitySqlTable m_routeItemsTable;
};
} // namespace md::data_source

#endif // MISSION_ITEMS_REPOSITORY_SQL_H
