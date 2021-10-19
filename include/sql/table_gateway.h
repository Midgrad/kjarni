#ifndef GENERIC_TABLE_H
#define GENERIC_TABLE_H

#include <QSqlQuery>
#include <QVariant>

namespace md
{
namespace data_source
{
namespace sql
{
const QString id = "id";
const QString hold = ":";
const QString comma = ", ";
} // namespace sql

using Condition = QPair<QString, QVariant>;
using ConditionMap = QVariantMap;

class TableGateway
{
public:
    TableGateway(QSqlDatabase* database, const QString& tableName);
    virtual ~TableGateway() = default;

    bool checkCreateTable();

    bool initColumnNames();
    void initColumnNames(const QStringList& columnNames);

    QString errorString() const;

    QList<QVariantMap> select(const ConditionMap& conditions = ConditionMap(),
                              const QStringList& resultColumns = QStringList()) const;
    QList<QVariantMap> select(const ConditionMap& conditions, const QStringList& resultColumns,
                              const QStringList& orderByColumns,
                              Qt::SortOrder sortOrder = Qt::AscendingOrder) const;

    bool insert(const QVariantMap& valueMap, QVariant* id = nullptr);

    bool removeByConditions(const ConditionMap& conditions);
    bool removeByCondition(const Condition& condition);
    bool removeById(const QVariant& id);

    bool updateByConditions(const QVariantMap& valueMap, const ConditionMap& conditions);
    bool updateByCondition(const QVariantMap& valueMap, const Condition& condition);
    bool updateById(const QVariantMap& valueMap, const QVariant& id);

    QString tableName() const;
    QStringList columnNames() const;

protected:
    QString prepareSelect(const QVariantMap& conditions, const QStringList& resultColumns = {},
                          const QStringList& sortColumns = {}, Qt::SortOrder sortOrder = {}) const;
    QString where(const QVariantMap& conditions) const;

private:
    QSqlDatabase* const m_database;
    const QString m_tableName;
    QStringList m_columnNames;
    mutable QString m_errorString;
};

} // namespace data_source
} // namespace md

#endif // GENERIC_TABLE_H
