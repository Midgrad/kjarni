#ifndef ROUTE_ITEM_TYPE_H
#define ROUTE_ITEM_TYPE_H

#include "parametrised.h"

namespace md::domain
{
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
