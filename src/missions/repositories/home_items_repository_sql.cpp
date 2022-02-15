#include "home_items_repository_sql.h"

#include <QDebug>

#include "mission_traits.h"

namespace
{
constexpr char routeItems[] = "home_items";
} // namespace

using namespace md::data_source;

HomeItemsRepositorySql::HomeItemsRepositorySql(QSqlDatabase* database) :
    domain::IHomeItemsRepository(),
    m_homeItemsTable(database, ::routeItems, { domain::props::params, domain::props::position })
{
}

QVariantMap HomeItemsRepositorySql::select(const QVariant& itemId)
{
    return m_homeItemsTable.selectById(itemId);
}

QVariant HomeItemsRepositorySql::selectMissionItemId(const QVariant& missionId)
{
    QVariantList list = m_homeItemsTable.selectOne({ { domain::props::mission, missionId } },
                                                   domain::props::id);
    return list.isEmpty() ? QVariant() : list.first();
}

void HomeItemsRepositorySql::insert(domain::RouteItem* item, const QVariant& missionId)
{
    QVariantMap map = m_homeItemsTable.entityToMap(item);

    map.insert(domain::props::mission, missionId);
    m_homeItemsTable.insert(map);
}

void HomeItemsRepositorySql::read(domain::RouteItem* item)
{
    m_homeItemsTable.readEntity(item);
}

void HomeItemsRepositorySql::update(domain::RouteItem* item)
{
    m_homeItemsTable.updateEntity(item);
}

void HomeItemsRepositorySql::remove(domain::RouteItem* item)
{
    m_homeItemsTable.removeEntity(item);
}

void HomeItemsRepositorySql::removeById(const QVariant& id)
{
    m_homeItemsTable.removeById(id);
}
