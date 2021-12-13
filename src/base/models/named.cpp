#include "named.h"

using namespace md::domain;

Named::Named(const QVariant& id, const QString& name, QObject* parent) :
    Entity(id, parent),
    name(name, std::bind(&Entity::changed, this))
{
}

QVariantMap Named::toVariantMap() const
{
    QVariantMap map = Entity::toVariantMap();
    map.insert(props::name, this->name());
    return map;
}

void Named::fromVariantMap(const QVariantMap& map)
{
    name = map.value(props::name, this->name()).toString();
    Entity::fromVariantMap(map);

    emit changed();
}
