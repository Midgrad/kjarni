#include "entity_repository_sql.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

using namespace md::domain;

EntityRepositorySql::EntityRepositorySql(QSqlDatabase* database, const QString& tableName) :
    IEntityRepository(),
    m_gateway(database, tableName)
{
    m_gateway.initColumnNames();

    if (m_gateway.errorString().length())
        qWarning() << m_gateway.errorString();
}

QVariantList EntityRepositorySql::selectIds(const QVariantMap& conditions) const
{
    QVariantList list;
    for (const QVariantMap& map : m_gateway.select(data_source::ConditionMap(conditions),
                                                   QStringList({ data_source::sql::id })))
    {
        list.append(map.value(data_source::sql::id));
    }
    if (m_gateway.errorString().length())
        qWarning() << m_gateway.errorString();

    return list;
}

QVariantMap EntityRepositorySql::select(const QVariant& id) const
{
    auto select = m_gateway.select({ { data_source::sql::id, id } }, m_gateway.columnNames());

    if (m_gateway.errorString().length())
        qWarning() << m_gateway.errorString();

    if (select.isEmpty())
        return QVariantMap();

    QVariantMap map = select.first();
    if (map.contains(params::params))
    {
        QJsonDocument doc = QJsonDocument::fromJson(map.value(params::params).toByteArray());
        QVariantMap params = doc.object().toVariantMap();
        map[params::params] = params;
    }
    return map;
}

void EntityRepositorySql::insert(Entity* entity)
{
    m_gateway.insert(this->entityToMap(entity));

    if (m_gateway.errorString().length())
        qWarning() << m_gateway.errorString();
}

void EntityRepositorySql::read(Entity* entity)
{
    entity->fromVariantMap(this->select(entity->id()));

    if (m_gateway.errorString().length())
        qWarning() << m_gateway.errorString();
}

void EntityRepositorySql::update(Entity* entity)
{
    m_gateway.updateById(entity->toVariantMap(false), entity->id());

    if (m_gateway.errorString().length())
        qWarning() << m_gateway.errorString();
}

void EntityRepositorySql::remove(Entity* entity)
{
    m_gateway.removeById(entity->id());

    if (m_gateway.errorString().length())
        qWarning() << m_gateway.errorString();
}

QVariantMap EntityRepositorySql::entityToMap(Entity* entity)
{
    QVariantMap map = entity->toVariantMap(false);

    if (map.contains(params::params))
    {
        QJsonObject json = QJsonObject::fromVariantMap(map.value(params::params).toMap());
        QJsonDocument doc(json);
        map[params::params] = doc.toJson();
    }
    return map;
}
