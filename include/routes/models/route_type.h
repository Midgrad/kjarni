#ifndef ROUTE_TYPE_H
#define ROUTE_TYPE_H

#include "route_item_type.h"
#include "route_pattern.h"

namespace md::domain
{
class RouteType
{
    Q_GADGET

public:
    RouteType(const QString& id, const QString& name,
              const QVector<const RouteItemType*>& itemTypes,
              const QVector<const RoutePattern*>& patterns);

    QVariantMap toVariantMap() const;

    const RouteItemType* itemType(const QString& id) const;

    const QString id;
    const QString name;
    const QMap<QString, const RouteItemType*> itemTypes;
    const QMap<QString, const RoutePattern*> patterns;
};
} // namespace md::domain

#endif // ROUTE_TYPE_H
