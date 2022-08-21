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

// Join QVariantList with commas and convert to QString
QString joinFromVariantList(const QVariantList& list);

// Split QString with commas and convert to QVariantList
QVariantList variantListFromJoin(const QString& str);

// Convert list to map with ids
template<typename T>
QMap<QString, T> vecToMap(const QVector<T>& list)
{
    QMap<QString, T> map;
    for (T type : list)
    {
        map[type->id] = type;
    }
    return map;
}
} // namespace md::utils

#endif // UTILS_H
