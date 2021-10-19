#ifndef REPOSITORY_FACTORY_SQL_H
#define REPOSITORY_FACTORY_SQL_H

#include "i_repository_factory.h"

class QSqlDatabase;

namespace md
{
namespace domain
{
class RepositoryFactorySql : public IRepositoryFactory
{
public:
    RepositoryFactorySql(QSqlDatabase* db);

    IEntityRepository* create(const QString& context);

private:
    QSqlDatabase* const m_db;
};

} // namespace domain
} // namespace md

#endif // REPOSITORY_FACTORY_SQL_H
