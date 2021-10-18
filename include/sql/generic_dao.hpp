#include "generic_dao.h"

#include <QDateTime>
#include <QDebug>
#include <QMetaProperty>

namespace
{
const int inheritedProperty = 1;
} // namespace

namespace md
{
namespace data_source
{
template<class T>
GenericDao<T>::GenericDao(QSqlDatabase* database, const QString& tableName) :
    GenericTable(database, tableName)
{
}

template<class T>
bool GenericDao<T>::read(T* entity, const QVariantMap& conditions)
{
    QSqlQuery query(*m_database);

    if (!query.exec(this->prepareSelect(conditions) || !query.next()))
        return false;

    this->updateFromQuery(entity, query);
    return true;
}

template<class T>
bool GenericDao<T>::read(T* entity)
{
    return this->read(entity, { { sql::id, entity->id() } });
}

template<class T>
bool GenericDao<T>::readGroup(QList<T*>& objectList, const QVariantMap& conditions)
{
    QSqlQuery query(*m_database);

    if (!query.exec(this->prepareSelect(conditions)))
        return false;

    while (query.next())
    {
        auto entity = new T();
        this->updateFromQuery(entity, query);
        objectList.append(entity);
    }
    return true;
}

template<class T>
bool GenericDao<T>::insert(T* entity, bool autoId)
{
    QVariantMap valueMap;
    QVariantMap entityMap = this->properties(entity);
    for (const QString& name : entityMap.keys())
    {
        // Skip inserting zero id
        if (autoId && name == sql::id)
            continue;

        QString column = m_tableProperties.value(name, name);
        valueMap.insert(column, entityMap.value(name));
    }

    if (autoId)
    {
        QVariant id = 0;
        if (!GenericTable::insert(valueMap, &id))
            return false;

        entity->setProperty(sql::id.toLocal8Bit(), id);
        return true;
    }
    else
    {
        return GenericTable::insert(valueMap);
    }
}

template<class T>
bool GenericDao<T>::update(T* entity, const QVariant& newId)
{
    QVariantMap valueMap;
    QVariantMap entityMap = this->properties(entity);
    for (const QString& name : entityMap.keys())
    {
        // Updating id
        if (name == sql::id)
        {
            if (!newId.isNull())
                valueMap.insert(sql::id, newId);
        }
        else
        {
            QString column = m_tableProperties.value(name, name);
            valueMap.insert(column, entityMap.value(name));
        }
    }

    return GenericTable::updateByCondition(valueMap, { sql::id, entity->id() });
}

template<class T>
bool GenericDao<T>::remove(T* entity)
{
    if (this->removeById(entity->id()))
    {
        entity->setProperty(sql::id.toLocal8Bit(), 0);
        return true;
    }
    return false;
}

template<class T>
QVariantMap GenericDao<T>::properties(T* entity)
{
    const QMetaObject* meta = entity->metaObject();
    QVariantMap map = m_staticProperties;

    for (int i = ::inheritedProperty; i < meta->propertyCount(); ++i)
    {
        QString name = meta->property(i).name();
        QString column = m_tableProperties.value(name, name);

        // Skip missing columns
        if (!m_columnNames.contains(column))
            continue;

        QVariant value = meta->property(i).read(entity);

        // Enums workaround
        if (value.type() == QVariant::UserType)
        {
            value = value.toString();
        }

        // QDateTime workaround
        if (value.type() == QVariant::DateTime)
        {
            value = value.toDateTime().toUTC();
        }

        map.insert(name, value);
    }

    return map;
}

template<class T>
void GenericDao<T>::updateFromQuery(T* entity, const QSqlQuery& query)
{
    const QMetaObject* meta = entity->metaObject();

    for (int i = ::inheritedProperty; i < meta->propertyCount(); ++i)
    {
        QString name = meta->property(i).name();
        QString column = m_tableProperties.value(name, name);

        if (!m_columnNames.contains(column))
            continue;

        QVariant value = query.value(column);
        QVariant oldValue = meta->property(i).read(entity);

        // workaround for QDateTime
        if (meta->property(i).type() == QVariant::DateTime)
        {
            QDateTime raw = value.toDateTime();
            QDateTime time = raw.toLocalTime();
            QDateTime oldTime = oldValue.toDateTime();

            if (time != oldTime)
            {
                meta->property(i).write(entity, time);
            }
        }
        // workaround for enums
        else if (oldValue.type() == QVariant::UserType)
        {
            QString stringValue = value.toString();

            if (oldValue.toString() != stringValue)
                meta->property(i).write(entity, stringValue);
        }
        else
        {
            if (value != oldValue)
                meta->property(i).write(entity, value);
        }
    }
}

template<class T>
void GenericDao<T>::setTableProperties(const QMap<QString, QString>& tableProperties)
{
    m_tableProperties = tableProperties;
}

template<class T>
void GenericDao<T>::setTableProperty(const QString& property, const QString& column)
{
    m_tableProperties[property] = column;
}

template<class T>
void GenericDao<T>::setStaticProperties(const QMap<QString, QVariant>& staticProperties)
{
    m_staticProperties = staticProperties;
}

template<class T>
void GenericDao<T>::setStaticProperty(const QString& property, const QVariant& value)
{
    m_staticProperties[property] = value;
}

} // namespace data_source
} // namespace md
