#include "mission_type.h"

using namespace md::domain;

MissionType::MissionType(const QString& id, const QString& name,
                         const MissionItemType* const homeItemType,
                         const QVector<const MissionItemType*>& itemTypes,
                         const QVector<const RoutePatternType*>& patternTypes) :
    id(id),
    name(name),
    homeItemType(homeItemType),
    itemTypes(utils::vecToMap<const MissionItemType*>(itemTypes)),
    patternTypes(utils::vecToMap<const RoutePatternType*>(patternTypes))
{
}

QVariantMap MissionType::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, id);
    map.insert(props::name, name);
    return map;
}

const MissionItemType* MissionType::itemType(const QString& id) const
{
    return this->itemTypes.value(id, nullptr);
}

const RoutePatternType* MissionType::patternType(const QString& id) const
{
    return this->patternTypes.value(id, nullptr);
}
