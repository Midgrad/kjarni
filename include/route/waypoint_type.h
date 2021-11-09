#ifndef WAYPOINT_TYPE_H
#define WAYPOINT_TYPE_H

#include "route_item_type.h"

namespace md::domain
{
class WaypointType : public RouteItemType
{
    Q_GADGET

public:
    WaypointType(const QString& id, const QString& name, const QString& shortName,
                 const QVector<const Parameter*>& parameters,
                 const QVector<const RouteItemType*>& itemTypes = {});

    const RouteItemType* itemType(const QString& id) const;

    const QMap<QString, const RouteItemType*> itemTypes;
};
} // namespace md::domain

#endif // WAYPOINT_TYPE_H
