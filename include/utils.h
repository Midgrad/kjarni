#ifndef UTILS_H
#define UTILS_H

#include <QJsonObject>
#include <QObject>

namespace kjarni::utils
{
void mergeJson(QJsonObject& src, const QJsonObject& other);
} // namespace kjarni::utils

#endif // UTILS_H
