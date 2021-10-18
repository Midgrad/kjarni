#ifndef ENTITY_REPOSITORY_SQL_H
#define ENTITY_REPOSITORY_SQL_H

#include "generic_dao.h"
#include "i_entity_repository.h"

namespace md
{
namespace data_source
{
template<class Entity>
class EntityRepositorySql : public IEntityRepository<Entity>
{
public:
    EntityRepositorySql(QSqlDatabase* database, const QString& tableName);

    QVariantList selectIds() const override;

    void create(Entity* entity) override;
    void read(Entity* entity) override;
    void update(Entity* entity) override;
    void remove(Entity* entity) override;

private:
    GenericDao<Entity> m_dao;
};

} // namespace data_source
} // namespace md

#endif // ENTITY_REPOSITORY_SQL_H
