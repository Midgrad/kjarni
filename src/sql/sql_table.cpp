#include "sql_table.h"

#include <QDebug>
#include <QSqlError>

namespace md
{
namespace data_source
{
SqlTable::SqlTable(QSqlDatabase* database, const QString& tableName) :
    m_database(database),
    m_tableName(tableName)
{
    QSqlQuery query(*m_database);

    query.exec("PRAGMA table_info(" + m_tableName + ")");
    if (query.lastError().type() != QSqlError::NoError)
        qWarning() << query.lastQuery() << query.lastError();

    while (query.next())
    {
        m_columnNames.append(query.value(1).toString());
    }
}

SqlTable::SqlTable(QSqlDatabase* database, const QString& tableName,
                   const QStringList& columnNames) :
    m_database(database),
    m_tableName(tableName)
{
    m_columnNames = columnNames;
}

QList<QVariantMap> SqlTable::select(const ConditionMap& conditions,
                                    const QStringList& resultColumns) const
{
    return this->select(conditions, resultColumns.isEmpty() ? this->columnNames() : resultColumns,
                        {}, {});
}

QList<QVariantMap> SqlTable::select(const ConditionMap& conditions,
                                    const QStringList& resultColumns,
                                    const QStringList& orderByColumns,
                                    Qt::SortOrder sortOrder) const
{
    QSqlQuery query(*m_database);

    bool result = query.exec(
        this->prepareSelect(conditions, resultColumns, orderByColumns, sortOrder));
    if (query.lastError().type() != QSqlError::NoError)
        qWarning() << query.lastQuery() << query.lastError();

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

QVariantList SqlTable::selectOne(const ConditionMap& conditions, const QString& resultColumn) const
{
    QSqlQuery query(*m_database);

    bool result = query.exec(this->prepareSelect(conditions, { resultColumn }));
    if (query.lastError().type() != QSqlError::NoError)
        qWarning() << query.lastQuery() << query.lastError();

    if (!result)
        return QVariantList();

    QVariantList list;
    while (query.next())
    {
        list.append(query.value(0));
    }
    return list;
}

bool SqlTable::insert(const QVariantMap& valueMap, QVariant* id)
{
    QVariantMap filtered = this->filterByColumns(valueMap);

    QSqlQuery query(*m_database);

    QStringList placeholders;
    for (const QString& name : filtered.keys())
    {
        placeholders.append(sql::hold + name);
    }

    QString namesJoin = filtered.keys().join(sql::comma);
    QString valuesJoin = placeholders.join(sql::comma);
    query.prepare("INSERT INTO " + m_tableName + " (" + namesJoin + ") VALUES (" + valuesJoin + ")");
    this->bind(query, filtered);

    bool result = query.exec();
    if (query.lastError().type() != QSqlError::NoError)
        qWarning() << query.lastQuery() << query.lastError();

    if (!result)
        return false;

    if (id)
        *id = query.lastInsertId();

    return true;
}

bool SqlTable::removeByConditions(const ConditionMap& conditions)
{
    if (conditions.isEmpty())
        return false;

    QSqlQuery query(*m_database);
    bool result = query.exec("DELETE FROM " + m_tableName + this->where(conditions));
    if (query.lastError().type() != QSqlError::NoError)
        qWarning() << query.lastQuery() << query.lastError();
    return result;
}

bool SqlTable::removeByCondition(const Condition& condition)
{
    return this->removeByConditions({ { condition.first, condition.second } });
}

bool SqlTable::updateByConditions(const QVariantMap& valueMap, const ConditionMap& conditions)
{
    if (conditions.isEmpty())
        return false;

    QVariantMap filtered = this->filterByColumns(valueMap);

    QStringList pairs;
    for (const QString& name : filtered.keys())
    {
        pairs.append(name + " = " + sql::hold + name);
    }

    QSqlQuery query(*m_database);
    query.prepare("UPDATE " + m_tableName + " SET " + pairs.join(sql::comma) +
                  this->where(conditions));
    this->bind(query, filtered);

    bool result = query.exec();
    if (query.lastError().type() != QSqlError::NoError)
        qWarning() << query.lastQuery() << query.lastError();
    return result;
}

bool SqlTable::updateByCondition(const QVariantMap& valueMap, const Condition& condition)
{
    return this->updateByConditions(valueMap, { { condition.first, condition.second } });
}

QString SqlTable::tableName() const
{
    return m_tableName;
}

QStringList SqlTable::columnNames() const
{
    return m_columnNames;
}

QString SqlTable::prepareSelect(const QVariantMap& conditions, const QStringList& resultColumns,
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

QString SqlTable::where(const QVariantMap& conditions) const
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

QVariantMap SqlTable::filterByColumns(const QVariantMap& valueMap)
{
    QVariantMap result;
    for (const QString& key : valueMap.keys())
    {
        if (m_columnNames.contains(key))
            result.insert(key, valueMap.value(key));
    }
    return result;
}

void SqlTable::bind(QSqlQuery& query, const QVariantMap& valueMap)
{
    for (const QString& name : valueMap.keys())
    {
        query.bindValue(sql::hold + name, valueMap.value(name));
    }
}

} // namespace data_source
} // namespace md
