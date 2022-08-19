#ifndef COMM_LINK_TYPES_REPOSITORY_SQL_H
#define COMM_LINK_TYPES_REPOSITORY_SQL_H

#include "entity_sql_table.h"
#include "i_comm_link_types_repository.h"

namespace md::data_source
{
class CommLinkTypesRepositorySql : public domain::ICommLinkTypesRepository
{
public:
    CommLinkTypesRepositorySql(QSqlDatabase* database);

    QList<const domain::CommLinkType*> selectCommLinkTypes() override;

    QVector<const domain::ParameterType*> selectParametersForType(const QString& id);

private:
    EntitySqlTable m_typesTable;
    EntitySqlTable m_paramsTable;
    EntitySqlTable m_paramTypesTable;
};
} // namespace md::data_source

#endif // COMM_LINK_TYPES_REPOSITORY_SQL_H
