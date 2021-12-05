#include "entity_sql_table.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

using namespace md::data_source;

EntitySqlTable::EntitySqlTable(QSqlDatabase* database, const QString& tableName) :
    SqlTable(database, tableName)
{
}

QVariantList EntitySqlTable::selectIds(const ConditionMap& conditions, const QString& column)
{
    return this->selectOne(ConditionMap(conditions), column);
}

QVariantMap EntitySqlTable::selectById(const QVariant& id, const QString& column)
{
    auto select = this->select({ { data_source::sql::id, id } }, this->columnNames());

    if (select.isEmpty())
        return QVariantMap();

    QVariantMap map = select.first();
    if (map.contains(domain::props::params))
    {
        QJsonDocument doc = QJsonDocument::fromJson(map.value(domain::props::params).toByteArray());
        QVariantMap params = doc.object().toVariantMap();
        map[domain::props::params] = params;
    }
    return map;
}

bool EntitySqlTable::removeById(const QVariant& id)
{
    return this->removeByCondition({ sql::id, id });
}

bool EntitySqlTable::updateById(const QVariantMap& valueMap, const QVariant& id)
{
    return this->updateByCondition(valueMap, { sql::id, id });
}

void EntitySqlTable::insertEntity(domain::Entity* entity)
{
    this->insert(this->entityToMap(entity));
}

void EntitySqlTable::readEntity(domain::Entity* entity)
{
    entity->fromVariantMap(this->selectById(entity->id));
}

void EntitySqlTable::updateEntity(domain::Entity* entity)
{
    this->updateById(this->entityToMap(entity), entity->id);
}

void EntitySqlTable::removeEntity(domain::Entity* entity)
{
    this->removeById(entity->id);
}

QVariantMap EntitySqlTable::entityToMap(domain::Entity* entity)
{
    QVariantMap map = entity->toVariantMap();

    if (map.contains(domain::props::params))
    {
        QJsonObject json = QJsonObject::fromVariantMap(map.value(domain::props::params).toMap());
        QJsonDocument doc(json);
        map[domain::props::params] = doc.toJson();
    }
    return map;
}
