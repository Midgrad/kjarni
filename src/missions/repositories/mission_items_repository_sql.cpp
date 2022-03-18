#include "mission_items_repository_sql.h"

#include <QDebug>

#include "mission_traits.h"

namespace
{
constexpr char missionItems[] = "mission_items";
} // namespace

using namespace md::data_source;

MissionItemsRepositorySql::MissionItemsRepositorySql(QSqlDatabase* database) :
    domain::IMissionItemsRepository(),
    m_routeItemsTable(database, ::missionItems, { domain::props::params, domain::props::position })
{
}

QVariantMap MissionItemsRepositorySql::select(const QVariant& itemId)
{
    return m_routeItemsTable.selectById(itemId);
}

QVariantList MissionItemsRepositorySql::selectMissionItemsIds(const QVariant& missionId)
{
    return m_routeItemsTable.selectOne({ { domain::props::mission, missionId } }, domain::props::id);
}

void MissionItemsRepositorySql::insert(domain::MissionRouteItem* item, const QVariant& missionId)
{
    QVariantMap map = m_routeItemsTable.entityToMap(item);

    map.insert(domain::props::mission, missionId);
    m_routeItemsTable.insert(map);
}

void MissionItemsRepositorySql::read(domain::MissionRouteItem* item)
{
    m_routeItemsTable.readEntity(item);
}

void MissionItemsRepositorySql::update(domain::MissionRouteItem* item)
{
    m_routeItemsTable.updateEntity(item);
}

void MissionItemsRepositorySql::remove(domain::MissionRouteItem* item)
{
    m_routeItemsTable.removeEntity(item);
}

void MissionItemsRepositorySql::removeById(const QVariant& id)
{
    m_routeItemsTable.removeById(id);
}
