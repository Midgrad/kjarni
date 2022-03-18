#include "mission_route_item.h"

#include <QDebug>

#include "mission_traits.h"

using namespace md::domain;

MissionRouteItem::MissionRouteItem(const MissionItemType* type, const QString& name,
                                   const QVariant& id, const QVariantMap& params,
                                   const Geodetic& position, QObject* parent) :
    TypedParametrisedMixin<NamedMixin<Entity>>(type->parameters.values().toVector(), params, name,
                                               id, parent),
    position(position, std::bind(&Entity::changed, this)),
    current(false, std::bind(&Entity::changed, this)),
    reached(false, std::bind(&Entity::changed, this)),
    m_type(type)
{
    Q_ASSERT(type);
}

MissionRouteItem::MissionRouteItem(const MissionItemType* type, const QVariantMap& map,
                                   QObject* parent) :
    MissionRouteItem(type, map.value(props::name).toString(), map.value(props::id),
                     map.value(props::params).toMap(), map.value(props::position).toMap(), parent)
{
}

QVariantMap MissionRouteItem::toVariantMap() const
{
    QVariantMap map = TypedParametrisedMixin<NamedMixin<Entity>>::toVariantMap();

    map.insert(props::type, m_type->id);

    map.insert(props::position, this->position().toVariantMap());
    map.insert(props::current, this->current());
    map.insert(props::reached, this->reached());

    return map;
}

void MissionRouteItem::fromVariantMap(const QVariantMap& map)
{
    position = map.value(props::position, this->position().toVariantMap()).toMap();
    current = map.value(props::current, this->current()).toBool();
    reached = map.value(props::reached, this->reached()).toBool();

    TypedParametrisedMixin<NamedMixin<Entity>>::fromVariantMap(map);
}

const MissionItemType* MissionRouteItem::type() const
{
    return m_type;
}

void MissionRouteItem::setType(const MissionItemType* type)
{
    Q_ASSERT(type);

    if (m_type == type)
        return;

    m_type = type;

    name = type->shortName;
    this->syncParameters();
}

void MissionRouteItem::syncParameters()
{
    this->resetTypeParameters(m_type->parameters.values().toVector());
}
