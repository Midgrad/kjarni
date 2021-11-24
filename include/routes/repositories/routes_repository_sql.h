#ifndef ROUTES_REPOSITORY_SQL_H
#define ROUTES_REPOSITORY_SQL_H

#include "entity_sql_table.h"
#include "i_routes_repository.h"

namespace md::data_source
{
class RoutesRepositorySql : public domain::IRoutesRepository
{
public:
    RoutesRepositorySql(QSqlDatabase* database);

    QVariantMap select(const QVariant& routeId) override;
    QVariantList selectRouteIds() override;

    void insert(domain::Route* route) override;
    void read(domain::Route* route) override;
    void update(domain::Route* route) override;
    void remove(domain::Route* route) override;

private:
    EntitySqlTable m_routesTable;
};
} // namespace md::data_source

#endif // ROUTES_REPOSITORY_SQL_H
