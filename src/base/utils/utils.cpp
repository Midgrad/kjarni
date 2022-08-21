#include "utils.h"

#include <QJsonArray>
#include <QStringList>
#include <QUuid>

namespace md::utils
{
constexpr char comma[] = ", ";

QVariant generateId()
{
    // NOTE: .toString() is a workaround, cause QVariant loses {} SOMETIMES
    return QUuid::createUuid().toString();
}

double guardNaN(const QVariant& variant)
{
    return variant.isNull() ? qQNaN() : variant.toDouble();
}

QJsonObject mergeJson(QJsonObject& src, const QJsonObject& other)
{
    for (auto it = other.constBegin(); it != other.constEnd(); ++it)
    {
        if (src.contains(it.key()))
        {
            if (src.value(it.key()).isObject() && other.value(it.key()).isObject())
            {
                QJsonObject one(src.value(it.key()).toObject());
                QJsonObject two(other.value(it.key()).toObject());

                src[it.key()] = mergeJson(one, two);
                continue;
            }
            else if (src.value(it.key()).isArray() && other.value(it.key()).isArray())
            {
                QJsonArray arr = other.value(it.key()).toArray();
                QJsonArray srcArr = src.value(it.key()).toArray();

                for (int i = 0; i < arr.size(); i++)
                {
                    if (!srcArr.contains(arr[i]))
                        srcArr.append(arr[i]);
                }
                src[it.key()] = srcArr;
                continue;
            }
        }

        src[it.key()] = it.value();
    }
    return src;
}

QJsonObject insertInArray(QJsonObject& src, const QString& path, const QJsonValue& value)
{
    QJsonArray array = src.value(path).toArray();
    array.append(value);
    src.insert(path, array);
    return src;
}

QString nameToId(const QString& name)
{
    QString result = name.toLower();
    return result.replace(' ', '_');
}

QString nameFromType(const QString& type, const QStringList& occupiedNames)
{
    QString name = type;

    for (int i = 1;; i++)
    {
        name = type + " " + QString::number(i);

        if (!occupiedNames.contains(name))
            break;
    }
    return name;
}

QVariantMap mergeMap(QVariantMap& dest, const QVariantMap& source)
{
    for (auto it = source.constBegin(); it != source.constEnd(); ++it)
    {
        dest[it.key()] = it.value();
    }
    return dest;
}

QString joinFromVariantList(const QVariantList& list)
{
    QStringList strings;
    for (const QVariant& var : list)
    {
        strings.append(var.toString());
    }
    return strings.join(comma);
}

QVariantList variantListFromJoin(const QString& str)
{
    QVariantList list;
    for (const QString& word : str.split(comma))
    {
        list.append(word);
    }
    return list;
}

} // namespace md::utils
