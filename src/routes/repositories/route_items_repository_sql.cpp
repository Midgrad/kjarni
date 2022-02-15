#include "route_items_repository_sql.h"

#include <QDebug>

#include "route_traits.h"

namespace
{
constexpr char routeItems[] = "route_items";
} // namespace

using namespace md::data_source;

RouteItemsRepositorySql::RouteItemsRepositorySql(QSqlDatabase* database) :
    domain::IRouteItemsRepository(),
    m_routeItemsTable(database, ::routeItems, { domain::props::params, domain::props::position })
{
}

QVariantMap RouteItemsRepositorySql::select(const QVariant& itemId)
{
    return m_routeItemsTable.selectById(itemId);
}

QVariantList RouteItemsRepositorySql::selectRouteItemsIds(const QVariant& routeId)
{
    return m_routeItemsTable.selectOne({ { domain::props::route, routeId } }, domain::props::id);
}

void RouteItemsRepositorySql::insert(domain::RouteItem* item, const QVariant& routeId)
{
    QVariantMap map = m_routeItemsTable.entityToMap(item);

    map.insert(domain::props::route, routeId);
    m_routeItemsTable.insert(map);
}

void RouteItemsRepositorySql::read(domain::RouteItem* item)
{
    m_routeItemsTable.readEntity(item);
}

void RouteItemsRepositorySql::update(domain::RouteItem* item)
{
    m_routeItemsTable.updateEntity(item);
}

void RouteItemsRepositorySql::remove(domain::RouteItem* item)
{
    m_routeItemsTable.removeEntity(item);
}

void RouteItemsRepositorySql::removeById(const QVariant& id)
{
    m_routeItemsTable.removeById(id);
}
