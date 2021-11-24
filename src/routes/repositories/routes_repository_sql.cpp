#include "routes_repository_sql.h"

#include <QDebug>

#include "route_traits.h"

namespace
{
constexpr char routes[] = "routes";
} // namespace

using namespace md::data_source;

RoutesRepositorySql::RoutesRepositorySql(QSqlDatabase* database) :
    domain::IRoutesRepository(),
    m_routesTable(database, ::routes)
{
}

QVariantMap RoutesRepositorySql::select(const QVariant& routeId)
{
    return m_routesTable.selectById(routeId);
}

QVariantList RoutesRepositorySql::selectRouteIds()
{
    return m_routesTable.selectIds();
}

void RoutesRepositorySql::insert(domain::Route* route)
{
    m_routesTable.insertEntity(route);
}

void RoutesRepositorySql::read(domain::Route* route)
{
    m_routesTable.readEntity(route);
}

void RoutesRepositorySql::update(domain::Route* route)
{
    m_routesTable.updateEntity(route);
}

void RoutesRepositorySql::remove(domain::Route* route)
{
    m_routesTable.removeEntity(route);
}
