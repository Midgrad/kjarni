#ifndef MISSION_TYPE_H
#define MISSION_TYPE_H

#include "mission_item_type.h"
#include "mission_pattern_type.h"

namespace md::domain
{
class MissionType
{
    Q_GADGET

public:
    MissionType(const QString& id, const QString& name,
                const MissionItemType* const homeItemType,
                const QVector<const MissionItemType*>& itemTypes,
                const QVector<const RoutePatternType*>& patternTypes);

    QVariantMap toVariantMap() const;

    const MissionItemType* itemType(const QString& id) const;
    const RoutePatternType* patternType(const QString& id) const;

    const QString id;
    const QString name;
    const MissionItemType* const homeItemType;
    const QMap<QString, const MissionItemType*> itemTypes;
    const QMap<QString, const RoutePatternType*> patternTypes;
};

} // namespace md::domain

#endif // MISSION_TYPE_H
