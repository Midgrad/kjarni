#ifndef GENERIC_DAO_H
#define GENERIC_DAO_H

#include "generic_table.h"

#include <QHash>

namespace md
{
namespace data_source
{
template<class T>
class GenericDao : public GenericTable
{
public:
    GenericDao(QSqlDatabase* database, const QString& tableName);

    bool read(T* entity, const QVariantMap& conditions);
    bool read(T* entity);
    bool readGroup(QList<T*>& objectList, const QVariantMap& conditions);
    bool insert(T* entity, bool autoId = true);
    bool update(T* entity, const QVariant& newId = QVariant());
    bool remove(T* entity);

    void setTableProperties(const QMap<QString, QString>& tableProperties);
    void setTableProperty(const QString& property, const QString& column);

    void setStaticProperties(const QMap<QString, QVariant>& staticProperties);
    void setStaticProperty(const QString& property, const QVariant& value);

protected:
    QVariantMap properties(T* entity);
    void updateFromQuery(T* entity, const QSqlQuery& query);

private:
    // TODO: refactor to PropertyTable
    QMap<QString, QString> m_tableProperties;
    QMap<QString, QVariant> m_staticProperties;
};

} // namespace data_source
} // namespace md

#include "generic_dao.hpp"

#endif // GENERIC_DAO_H
