#ifndef SQL_TABLE_H
#define SQL_TABLE_H

#include <QSqlQuery>
#include <QVariant>

namespace md
{
namespace data_source
{
namespace sql
{
const QString hold = ":";
const QString comma = ", ";
} // namespace sql

using Condition = QPair<QString, QVariant>;
using ConditionMap = QVariantMap;

class SqlTable
{
public:
    SqlTable(QSqlDatabase* database, const QString& tableName);
    SqlTable(QSqlDatabase* database, const QString& tableName, const QStringList& columnNames);
    virtual ~SqlTable() = default;

    QList<QVariantMap> select(const ConditionMap& conditions = ConditionMap(),
                              const QStringList& resultColumns = QStringList()) const;
    QList<QVariantMap> select(const ConditionMap& conditions, const QStringList& resultColumns,
                              const QStringList& orderByColumns,
                              Qt::SortOrder sortOrder = Qt::AscendingOrder) const;

    bool insert(const QVariantMap& valueMap, QVariant* id = nullptr);

    bool removeByConditions(const ConditionMap& conditions);
    bool removeByCondition(const Condition& condition);

    bool updateByConditions(const QVariantMap& valueMap, const ConditionMap& conditions);
    bool updateByCondition(const QVariantMap& valueMap, const Condition& condition);

    QString tableName() const;
    QStringList columnNames() const;

protected:
    QString prepareSelect(const QVariantMap& conditions, const QStringList& resultColumns = {},
                          const QStringList& sortColumns = {}, Qt::SortOrder sortOrder = {}) const;
    QString where(const QVariantMap& conditions) const;

    void bind(QSqlQuery& query, const QVariantMap& valueMap);

private:
    QSqlDatabase* const m_database;
    const QString m_tableName;
    QStringList m_columnNames;
};

} // namespace data_source
} // namespace md

#endif // SQL_TABLE_H
