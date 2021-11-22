#ifndef SQLITE_SCHEMA_H
#define SQLITE_SCHEMA_H

#include "i_sql_schema.h"

namespace md::data_source
{
class SqliteSchema : public ISqlSchema
{
public:
    SqliteSchema(const QString& databaseName);

    QSqlDatabase* db() override;

    void setup() override;

private:
    QSqlDatabase m_db;
};
} // namespace md::data_source

#endif // SQLITE_SCHEMA_H
