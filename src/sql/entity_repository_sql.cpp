#include "entity_repository_sql.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

using namespace md::domain;

EntityRepositorySql::EntityRepositorySql(QSqlDatabase* database, const QString& tableName) :
    IEntityRepository(),
    m_gateway(database, tableName)
{
    m_gateway.initColumnNames();

    if (m_gateway.errorString().length())
        qWarning() << m_gateway.errorString();
}

QVariantList EntityRepositorySql::selectIds() const
{
    QVariantList list;
    for (const QVariantMap& map :
         m_gateway.select(data_source::ConditionMap(), QStringList({ data_source::sql::id })))
    {
        list.append(map.value(data_source::sql::id));
    }
    if (m_gateway.errorString().length())
        qWarning() << m_gateway.errorString();

    return list;
}

QVariantMap EntityRepositorySql::select(const QVariant& id) const
{
    QVariantMap result = m_gateway.select({ { data_source::sql::id, id } }, m_gateway.columnNames())
                             .first();

    if (m_gateway.errorString().length())
        qWarning() << m_gateway.errorString();

    return result;
}

void EntityRepositorySql::insert(Entity* entity)
{
    qDebug() << entity->toVariantMap(false);
    m_gateway.insert(entity->toVariantMap(false));

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
