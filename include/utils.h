#ifndef UTILS_H
#define UTILS_H

#include <QJsonObject>

namespace kjarni::utils
{
// Merge two JSON in one
QJsonObject mergeJson(QJsonObject& src, const QJsonObject& other);

// Insert value in array
QJsonObject insertInArray(QJsonObject& src, const QString& path, const QJsonValue& value);

// Name to filename
QString nameToFilename(const QString& name, const QString& extension);
} // namespace kjarni::utils

#endif // UTILS_H
