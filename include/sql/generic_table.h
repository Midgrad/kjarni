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

class GenericTable
{
public:
    GenericTable(QSqlDatabase* database, const QString& tableName);
    virtual ~GenericTable() = default;

    bool initColumnNames();
    void initColumnNames(const QStringList& columnNames);

    QVariantList select(const ConditionMap& conditions,
                        const QStringList& resultColumns = QStringList());
    QVariantList selectAll(const QStringList& resultColumns = QStringList());

    QVariantList orderedSelect(const ConditionMap& conditions, const QStringList& resultColumns,
                               const QStringList& orderByColumns,
                               Qt::SortOrder sortOrder = Qt::AscendingOrder);

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
                          const QStringList& sortColumns = {}, Qt::SortOrder sortOrder = {});
    QString where(const QVariantMap& conditions) const;

    QSqlDatabase* const m_database;
    const QString m_tableName;
    QStringList m_columnNames;
};

} // namespace data_source
} // namespace md

#endif // GENERIC_TABLE_H
