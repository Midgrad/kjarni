#include "mission_items_repository_sql.h"

#include <QDebug>

#include "mission_traits.h"

namespace
{
constexpr char missionRouteItems[] = "mission_route_items";
} // namespace

using namespace md::data_source;

MissionItemsRepositorySql::MissionItemsRepositorySql(QSqlDatabase* database) :
    domain::IMissionItemsRepository(),
    m_missionItemsTable(database, ::missionRouteItems,
                        { domain::props::params, domain::props::position })
{
}

QVariantMap MissionItemsRepositorySql::select(const QVariant& itemId)
{
    return m_missionItemsTable.selectById(itemId);
}

QVariantList MissionItemsRepositorySql::selectMissionItemsIds(const QVariant& missionId)
{
    return m_missionItemsTable.selectOne({ { domain::props::mission, missionId } },
                                         domain::props::id);
}

void MissionItemsRepositorySql::insert(domain::MissionRouteItem* item, const QVariant& missionId)
{
    QVariantMap map = m_missionItemsTable.entityToMap(item);

    map.insert(domain::props::mission, missionId);
    m_missionItemsTable.insert(map);
}

void MissionItemsRepositorySql::read(domain::MissionRouteItem* item)
{
    m_missionItemsTable.readEntity(item);
}

void MissionItemsRepositorySql::update(domain::MissionRouteItem* item)
{
    m_missionItemsTable.updateEntity(item);
}

void MissionItemsRepositorySql::remove(domain::MissionRouteItem* item)
{
    m_missionItemsTable.removeEntity(item);
}

void MissionItemsRepositorySql::removeById(const QVariant& id)
{
    m_missionItemsTable.removeById(id);
}
