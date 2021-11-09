#ifndef ROUTE_TYPE_H
#define ROUTE_TYPE_H

#include "route_item_type.h"

namespace md::domain
{
class RouteType
{
    Q_GADGET

public:
    RouteType(const QString& id, const QString& name,
              const QVector<const RouteItemType*>& waypointTypes);

    const RouteItemType* waypointType(const QString& id) const;

    const QString id;
    const QString name;
    const QMap<QString, const RouteItemType*> waypointTypes;
};
} // namespace md::domain

#endif // ROUTE_TYPE_H
