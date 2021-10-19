#include "table_gateway.h"

#include <QDebug>
#include <QSqlError>

namespace md
{
namespace data_source
{
TableGateway::TableGateway(QSqlDatabase* database, const QString& tableName) :
    m_database(database),
    m_tableName(tableName)
{
}

bool TableGateway::initColumnNames()
{
    QSqlQuery query(*m_database);

    bool result = query.exec("PRAGMA table_info(" + m_tableName + ")");
    m_errorString = query.lastError().text();

    if (!result)
        return false;

    while (query.next())
    {
        m_columnNames.append(query.value(1).toString());
    }

    return true;
}

void TableGateway::initColumnNames(const QStringList& columnNames)
{
    m_columnNames = columnNames;
}

QString TableGateway::errorString() const
{
    return m_errorString;
}

QList<QVariantMap> TableGateway::select(const ConditionMap& conditions,
                                        const QStringList& resultColumns) const
{
    return this->select(conditions, resultColumns.isEmpty() ? this->columnNames() : resultColumns,
                        {}, {});
}

QList<QVariantMap> TableGateway::select(const ConditionMap& conditions,
                                        const QStringList& resultColumns,
                                        const QStringList& orderByColumns,
                                        Qt::SortOrder sortOrder) const
{
    QSqlQuery query(*m_database);

    bool result = query.exec(
        this->prepareSelect(conditions, resultColumns, orderByColumns, sortOrder));
    m_errorString = query.lastError().text();

    if (!result)
        return QList<QVariantMap>();

    QList<QVariantMap> map;
    while (query.next())
    {
        QVariantMap values;
        for (const QString& column : resultColumns)
        {
            values.insert(column, query.value(column));
        }
        map.append(values);
    }
    return map;
}

bool TableGateway::insert(const QVariantMap& valueMap, QVariant* id)
{
    QSqlQuery query(*m_database);

    QStringList placeholders;
    for (const QString& name : valueMap.keys())
    {
        placeholders.append(sql::hold + name);
    }

    QString namesJoin = valueMap.keys().join(sql::comma);
    QString valuesJoin = placeholders.join(sql::comma);
    query.prepare("INSERT INTO " + m_tableName + " (" + namesJoin + ") VALUES (" + valuesJoin + ")");

    for (const QString& name : valueMap.keys())
    {
        query.bindValue(sql::hold + name, valueMap.value(name));
    }

    bool result = query.exec();

    qDebug() << query.lastQuery();
    m_errorString = query.lastError().text();

    if (!result)
        return false;

    if (id)
        *id = query.lastInsertId();

    return true;
}

bool TableGateway::removeByConditions(const ConditionMap& conditions)
{
    if (conditions.isEmpty())
        return false;

    QSqlQuery query(*m_database);
    bool result = query.exec("DELETE FROM " + m_tableName + this->where(conditions));
    m_errorString = query.lastError().text();
    return result;
}

bool TableGateway::removeByCondition(const Condition& condition)
{
    return this->removeByConditions({ { condition.first, condition.second } });
}

bool TableGateway::removeById(const QVariant& id)
{
    return this->removeByCondition({ sql::id, id });
}

bool TableGateway::updateByConditions(const QVariantMap& valueMap, const ConditionMap& conditions)
{
    if (conditions.isEmpty())
        return false;

    QStringList pairs;
    for (const QString& name : valueMap.keys())
    {
        // TODO: ignore names missmatching table columns
        pairs.append(name + " = " + sql::hold + name);
    }

    QSqlQuery query(*m_database);
    query.prepare("UPDATE " + m_tableName + " SET " + pairs.join(sql::comma) +
                  this->where(conditions));
    for (const QString& name : valueMap.keys())
    {
        query.bindValue(sql::hold + name, valueMap.value(name));
    }

    bool result = query.exec();
    m_errorString = query.lastError().text();
    return result;
}

bool TableGateway::updateByCondition(const QVariantMap& valueMap, const Condition& condition)
{
    return this->updateByConditions(valueMap, { { condition.first, condition.second } });
}

bool TableGateway::updateById(const QVariantMap& valueMap, const QVariant& id)
{
    return this->updateByCondition(valueMap, { sql::id, id });
}

QString TableGateway::tableName() const
{
    return m_tableName;
}

QStringList TableGateway::columnNames() const
{
    return m_columnNames;
}

QString TableGateway::prepareSelect(const QVariantMap& conditions, const QStringList& resultColumns,
                                    const QStringList& sortColumns, Qt::SortOrder sortOrder) const
{
    QString queryString = "SELECT " +
                          (resultColumns.isEmpty() ? "*" : resultColumns.join(sql::comma)) +
                          " FROM " + m_tableName;
    if (!conditions.isEmpty())
        queryString += this->where(conditions);
    if (!sortColumns.isEmpty())
        queryString += QString(" ORDER BY ") + sortColumns.join(", ") +
                       (sortOrder == Qt::AscendingOrder ? " ASC" : " DESC");

    return queryString;
}

QString TableGateway::where(const QVariantMap& conditions) const
{
    QStringList conditionList;
    for (const QString& key : conditions.keys())
    {
        QVariant value = conditions.value(key);
        if (value.isNull())
        {
            conditionList.append(key + " IS NULL");
        }
        else
        {
            conditionList.append(key + " = \'" + value.toString() + "\'");
        }
    }
    return QString(" WHERE ") + conditionList.join(" AND ");
}

} // namespace data_source
} // namespace md
