#include "generic_table.h"

#include <QDebug>
#include <QSqlError>

namespace md
{
namespace data_source
{
GenericTable::GenericTable(QSqlDatabase* database, const QString& tableName) :
    m_database(database),
    m_tableName(tableName)
{
}

bool GenericTable::initColumnNames()
{
    QSqlQuery query(*m_database);

    if (!query.exec("PRAGMA table_info(" + m_tableName + ")"))
        return false;

    while (query.next())
    {
        m_columnNames.append(query.value(1).toString());
    }

    return true;
}

void GenericTable::initColumnNames(const QStringList& columnNames)
{
    m_columnNames = columnNames;
}

QVariantList GenericTable::select(const ConditionMap& conditions, const QStringList& resultColumns)
{
    return this->orderedSelect(conditions, resultColumns, {}, {});
}

QVariantList GenericTable::selectAll(const QStringList& resultColumns)
{
    return this->select(ConditionMap(), resultColumns);
}

QVariantList GenericTable::orderedSelect(const ConditionMap& conditions,
                                         const QStringList& resultColumns,
                                         const QStringList& orderByColumns, Qt::SortOrder sortOrder)
{
    QSqlQuery query(*m_database);

    if (!query.exec(this->prepareSelect(conditions, resultColumns, orderByColumns, sortOrder)))
        return QVariantList();

    QVariantList resultList;
    while (query.next())
    {
        if (resultColumns.count() > 1)
        {
            QVariantList valueList;
            for (const QString& column : resultColumns)
            {
                valueList.append(query.value(column));
            }
            resultList.append(QVariant(valueList));
        }
        else
        {
            resultList.append(query.value(resultColumns.first()));
        }
    }
    return resultList;
}

bool GenericTable::insert(const QVariantMap& valueMap, QVariant* id)
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

    if (!query.exec())
        return false;

    if (id)
        *id = query.lastInsertId();

    return true;
}

bool GenericTable::removeByConditions(const ConditionMap& conditions)
{
    if (conditions.isEmpty())
        return false;

    QSqlQuery query(*m_database);
    return query.exec("DELETE FROM " + m_tableName + this->where(conditions));
}

bool GenericTable::removeByCondition(const Condition& condition)
{
    return this->removeByConditions({ { condition.first, condition.second } });
}

bool GenericTable::removeById(const QVariant& id)
{
    return this->removeByCondition({ sql::id, id });
}

bool GenericTable::updateByConditions(const QVariantMap& valueMap, const ConditionMap& conditions)
{
    if (conditions.isEmpty())
        return false;

    QStringList pairs;
    for (const QString& name : valueMap.keys())
    {
        pairs.append(name + " = " + sql::hold + name);
    }

    QSqlQuery query(*m_database);
    query.prepare("UPDATE " + m_tableName + " SET " + pairs.join(sql::comma) +
                  this->where(conditions));
    for (const QString& name : valueMap.keys())
    {
        query.bindValue(sql::hold + name, valueMap.value(name));
    }

    if (!query.exec())
        return false;

    return true;
}

bool GenericTable::updateByCondition(const QVariantMap& valueMap, const Condition& condition)
{
    return this->updateByConditions(valueMap, { { condition.first, condition.second } });
}

bool GenericTable::updateById(const QVariantMap& valueMap, const QVariant& id)
{
    return this->updateByCondition(valueMap, { sql::id, id });
}

QString GenericTable::tableName() const
{
    return m_tableName;
}

QStringList GenericTable::columnNames() const
{
    return m_columnNames;
}

QString GenericTable::prepareSelect(const QVariantMap& conditions, const QStringList& resultColumns,
                                    const QStringList& sortColumns, Qt::SortOrder sortOrder)
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

QString GenericTable::where(const QVariantMap& conditions) const
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
