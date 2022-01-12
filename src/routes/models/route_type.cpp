#include "route_type.h"

#include "route_traits.h"
#include "utils.h"

using namespace md::domain;

RouteType::RouteType(const QString& id, const QString& name,
                     const QVector<const RouteItemType*>& itemTypes,
                     const QVector<const RoutePattern*>& patterns) :
    id(id),
    name(name),
    itemTypes(utils::listToMap<RouteItemType>(itemTypes)),
    patterns(utils::listToMap<RoutePattern>(patterns))
{
}

QVariantMap RouteType::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, id);
    map.insert(props::name, name);
    return map;
}

const RouteItemType* RouteType::itemType(const QString& id) const
{
    return this->itemTypes.value(id, nullptr);
}

const RoutePattern* RouteType::pattern(const QString& id) const
{
    return this->patterns.value(id, nullptr);
}
