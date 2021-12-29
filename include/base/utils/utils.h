#ifndef UTILS_H
#define UTILS_H

#include <QJsonObject>
#include <QVariant>

namespace md::utils
{
// Generate entity id
QVariant generateId();

// return NaN if varinat is null
double guardNaN(const QVariant& variant);

// Merge two JSON in one
QJsonObject mergeJson(QJsonObject& src, const QJsonObject& other);

// Insert value in array
QJsonObject insertInArray(QJsonObject& src, const QString& path, const QJsonValue& value);

// Name to filename
QString nameToId(const QString& name);

// Generate numerated name from type
QString nameFromType(const QString& type, const QStringList& occupiedNames);

// Merge 2 varinat maps to one
QVariantMap mergeMap(QVariantMap& dest, const QVariantMap& source);

// Convert list to map with ids
template<typename T>
QMap<QString, const T*> listToMap(const QVector<const T*>& list)
{
    QMap<QString, const T*> map;
    for (const T* type : list)
    {
        map[type->id] = type;
    }
    return map;
}
} // namespace md::utils

#endif // UTILS_H
