#ifndef UTILS_H
#define UTILS_H

#include <QJsonObject>

namespace md::utils
{
// Merge two JSON in one
QJsonObject mergeJson(QJsonObject& src, const QJsonObject& other);

// Insert value in array
QJsonObject insertInArray(QJsonObject& src, const QString& path, const QJsonValue& value);

// Name to filename
QString nameToId(const QString& name);

// Generate numerated name from type
QString nameFromType(const QString& type, const QStringList& occupiedNames);
} // namespace md::utils

#endif // UTILS_H
