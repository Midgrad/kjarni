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
    m_routeItemsTable(database, ::routeItems)
{
}

QVariantMap RouteItemsRepositorySql::select(const QVariant& itemId)
{
    return m_routeItemsTable.selectById(itemId);
}

QVariantList RouteItemsRepositorySql::selectChildItemsIds(const QVariant& parentId)
{
    return m_routeItemsTable.selectOne({ { domain::props::parent, parentId } }, domain::props::id);
}

void RouteItemsRepositorySql::insert(domain::RouteItem* item, const QVariant& parentId)
{
    QVariantMap map = m_routeItemsTable.entityToMap(item);

    if (!parentId.isNull())
        map.insert(domain::props::parent, parentId);

    m_routeItemsTable.insert(map);
}

void RouteItemsRepositorySql::read(domain::RouteItem* item)
{
    m_routeItemsTable.readEntity(item);
}

void RouteItemsRepositorySql::update(domain::RouteItem* item, const QVariant& parentId)
{
    QVariantMap map = m_routeItemsTable.entityToMap(item);

    if (!parentId.isNull())
        map.insert(domain::props::parent, parentId);

    m_routeItemsTable.updateById(map, item->id());
}

void RouteItemsRepositorySql::remove(domain::RouteItem* item)
{
    m_routeItemsTable.removeEntity(item);
}

void RouteItemsRepositorySql::removeById(const QVariant& id)
{
    m_routeItemsTable.removeById(id);
}
