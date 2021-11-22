#include "entity.h"

using namespace md::domain;

Entity::Entity(const QVariant& id, QObject* parent) : QObject(parent), m_id(id)
{
}

Entity::Entity(const QVariantMap& map, QObject* parent) :
    Entity(map.value(props::id, utils::generateId()), parent)
{
}

Entity::~Entity()
{
}

QVariant Entity::id() const
{
    return m_id;
}

QVariantMap Entity::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, m_id.toString());
    return map;
}

void Entity::fromVariantMap(const QVariantMap& map)
{
    Q_UNUSED(map)
}
