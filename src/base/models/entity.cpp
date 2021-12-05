#include "entity.h"

using namespace md::domain;

Entity::Entity(const QVariant& id, QObject* parent) : QObject(parent), id(id)
{
}

Entity::~Entity()
{
}

QVariantMap Entity::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, id);
    return map;
}

void Entity::fromVariantMap(const QVariantMap& map)
{
    Q_UNUSED(map)
}
