#ifndef ROUTE_ITEM_TYPE_H
#define ROUTE_ITEM_TYPE_H

#include <QVariant>
#include <QVector>

namespace md::domain
{
class Parameter // TODO: parameters to bounded context, Parametered
{
    Q_GADGET

public:
    enum Type
    {
        Bool,
        Int,
        Real
    };

    Parameter(const QString& id, const QString& name, Type type, const QVariant& defaultValue,
              const QVariant& minValue, const QVariant& maxValue, const QVariant& step = 1);
    Parameter(const QString& id, const QString& name, bool defaultValue = false);

    QVariantMap toVariantMap() const;
    QVariant guard(const QVariant& value) const;

    const QString id;
    const QString name;
    const Type type;
    const QVariant defaultValue;
    const QVariant minValue;
    const QVariant maxValue;
    const QVariant step;

    Q_ENUM(Type)
};

class RouteItem;
class RouteItemType
{
    Q_GADGET

public:
    RouteItemType(const QString& id, const QString& name, const QString& shortName,
                  const QVector<const Parameter*>& parameters);

    QVariantMap toVariantMap() const;

    const Parameter* parameter(const QString& id) const;
    QVariantMap defaultParameters() const;

    const QString id;
    const QString name;
    const QString shortName;
    const QMap<QString, const Parameter*> parameters;
};
} // namespace md::domain

#endif // ROUTE_ITEM_TYPE_H
