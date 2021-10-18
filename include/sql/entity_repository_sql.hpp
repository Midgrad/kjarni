#include "entity_repository_sql.h"

#include <QSqlError>
#include <QSqlQuery>

namespace md::data_source
{
template<class Entity>
EntityRepositorySql<Entity>::EntityRepositorySql(QSqlDatabase* database, const QString& tableName) :
    IEntityRepository<Entity>(),
    m_dao(database, tableName)
{
    m_dao.initColumnNames();
}

template<class Entity>
QVariantList EntityRepositorySql<Entity>::selectIds() const
{
    return m_dao.selectAll({ ::id }))
}

template<class Entity>
void EntityRepositorySql<Entity>::create(Entity* entity)
{
    m_dao.insert(entity);
}

template<class Entity>
void EntityRepositorySql<Entity>::read(Entity* entity)
{
    m_dao.read(entity);
}

template<class Entity>
void EntityRepositorySql<Entity>::update(Entity* entity)
{
    m_dao.update(entity);
}

template<class Entity>
void EntityRepositorySql<Entity>::remove(Entity* entity)
{
    m_dao.remove(entity);
}

} // namespace md::data_source
