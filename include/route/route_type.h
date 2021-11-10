#ifndef ROUTE_TYPE_H
#define ROUTE_TYPE_H

#include "waypoint_type.h"

namespace md::domain
{
class RouteType
{
    Q_GADGET

public:
    RouteType(const QString& id, const QString& name,
              const QVector<const WaypointType*>& waypointTypes);

    QVariantMap toVariantMap() const;

    const WaypointType* waypointType(const QString& id) const;

    const QString id;
    const QString name;
    const QMap<QString, const WaypointType*> waypointTypes;
};
} // namespace md::domain

#endif // ROUTE_TYPE_H
