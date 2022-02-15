#include "route_item.h"

#include <QDebug>

#include "route_traits.h"

using namespace md::domain;

RouteItem::RouteItem(const RouteItemType* type, const QString& name, const QVariant& id,
                     const QVariantMap& params, const Geodetic& position, bool current,
                     bool reached, QObject* parent) :
    TypedParametrisedMixin<NamedMixin<Entity>>(type->parameters.values().toVector(), params, name,
                                               id, parent),
    position(position, std::bind(&Entity::changed, this)),
    current(current, std::bind(&Entity::changed, this)),
    reached(reached, std::bind(&Entity::changed, this)),
    m_type(type)
{
    Q_ASSERT(type);
}

RouteItem::RouteItem(const RouteItemType* type, const QVariantMap& map, QObject* parent) :
    RouteItem(type, map.value(props::name).toString(), map.value(props::id),
              map.value(props::params).toMap(), map.value(props::position).toMap(),
              map.value(props::current).toBool(), map.value(props::reached).toBool(), parent)
{
}

QVariantMap RouteItem::toVariantMap() const
{
    QVariantMap map = TypedParametrisedMixin<NamedMixin<Entity>>::toVariantMap();

    map.insert(props::type, m_type->id);

    map.insert(props::position, this->position().toVariantMap());
    map.insert(props::current, this->current());
    map.insert(props::reached, this->reached());

    return map;
}

void RouteItem::fromVariantMap(const QVariantMap& map)
{
    position = map.value(props::position, this->position().toVariantMap()).toMap();
    current = map.value(props::current, this->current()).toBool();
    reached = map.value(props::reached, this->reached()).toBool();

    TypedParametrisedMixin<NamedMixin<Entity>>::fromVariantMap(map);
}

const RouteItemType* RouteItem::type() const
{
    return m_type;
}

void RouteItem::setType(const RouteItemType* type)
{
    Q_ASSERT(type);

    if (m_type == type)
        return;

    m_type = type;

    name = type->shortName;
    this->syncParameters();
}

void RouteItem::syncParameters()
{
    this->resetTypeParameters(m_type->parameters.values().toVector());
}
