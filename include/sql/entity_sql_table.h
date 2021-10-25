#ifndef ENTITY_SQL_TABLE_H
#define ENTITY_SQL_TABLE_H

#include "entity.h"
#include "sql_table.h"

namespace md
{
namespace data_source
{
namespace sql
{
const QString id = "id";
}

class EntitySqlTable : public SqlTable
{
public:
    EntitySqlTable(QSqlDatabase* database, const QString& tableName);

    QVariantList selectIds(const ConditionMap& conditions = ConditionMap(),
                           const QString& column = sql::id);
    QVariantMap selectById(const QVariant& id, const QString& column = sql::id);

    bool removeById(const QVariant& id);
    bool updateById(const QVariantMap& valueMap, const QVariant& id);

    void insertEntity(domain::Entity* entity);
    void readEntity(domain::Entity* entity);
    void updateEntity(domain::Entity* entity);
    void removeEntity(domain::Entity* entity);

protected:
    QVariantMap entityToMap(domain::Entity* entity);
};

} // namespace data_source
} // namespace md

#endif // ENTITY_SQL_TABLE_H
