#ifndef ENTITY_REPOSITORY_SQL_H
#define ENTITY_REPOSITORY_SQL_H

#include "i_entity_repository.h"
#include "table_gateway.h"

namespace md
{
namespace domain
{
class EntityRepositorySql : public IEntityRepository
{
public:
    EntityRepositorySql(QSqlDatabase* database, const QString& tableName);

    QVariantList selectIds(const QVariantMap& conditions) const override;
    QVariantMap select(const QVariant& id) const override;

    void insert(Entity* entity) override;
    void read(Entity* entity) override;
    void update(Entity* entity) override;
    void remove(Entity* entity) override;

private:
    data_source::TableGateway m_gateway;
};

} // namespace domain
} // namespace md

#endif // ENTITY_REPOSITORY_SQL_H
