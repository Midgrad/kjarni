#include "repository_factory_sql.h"

#include "entity_repository_sql.h"

using namespace md::domain;

RepositoryFactorySql::RepositoryFactorySql(QSqlDatabase* db) : m_db(db)
{
}

IEntityRepository* md::domain::RepositoryFactorySql::create(const QString& context)
{
    // Use context as table name
    return new EntityRepositorySql(m_db, context);
}
