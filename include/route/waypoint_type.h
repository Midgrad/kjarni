#ifndef WAYPOINT_TYPE_H
#define WAYPOINT_TYPE_H

#include "waypoint_item_type.h"

namespace md::domain
{
class WaypointType : public WaypointItemType
{
    Q_GADGET

public:
    WaypointType(const QString& id, const QString& name, const QString& shortName,
                 const QVector<const Parameter*>& parameters,
                 const QVector<const WaypointItemType*>& itemTypes = {});

    const WaypointItemType* itemType(const QString& id) const;

    const QMap<QString, const WaypointItemType*> itemTypes;
};
} // namespace md::domain

#endif // WAYPOINT_TYPE_H
