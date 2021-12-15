#include "entity_sql_table.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

namespace
{
QVariantMap fromJson(const QVariant& content)
{
    QJsonDocument doc = QJsonDocument::fromJson(content.toByteArray());
    return doc.object().toVariantMap();
}

QByteArray toJson(const QVariant& content)
{
    QJsonObject json = QJsonObject::fromVariantMap(content.toMap());
    QJsonDocument doc(json);
    return doc.toJson();
}

} // namespace

using namespace md::data_source;

EntitySqlTable::EntitySqlTable(QSqlDatabase* database, const QString& tableName,
                               const QStringList& jsonProperties) :
    SqlTable(database, tableName),
    m_jsonProperties(jsonProperties)
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
    for (const QString& property : m_jsonProperties)
    {
        if (map.contains(property))
        {
            map[property] = ::fromJson(map.value(property));
        }
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

    for (const QString& property : m_jsonProperties)
    {
        if (map.contains(property))
        {
            map[property] = ::toJson(map.value(property));
        }
    }
    return map;
}
