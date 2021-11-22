#ifndef I_SQL_SCHEMA_H
#define I_SQL_SCHEMA_H

#include <QSqlDatabase>

namespace md::data_source
{
class ISqlSchema
{
public:
    ISqlSchema() = default;
    virtual ~ISqlSchema() = default;

    virtual QSqlDatabase* db() = 0;

    virtual void setup() = 0;
};
} // namespace md::data_source

#endif // I_SQL_SCHEMA_H
