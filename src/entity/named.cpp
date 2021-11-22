#include "named.h"

using namespace md::domain;

Named::Named(const QVariant& id, const QString& name, QObject* parent) :
    Entity(id, parent),
    m_name(name)
{
}

Named::Named(const QVariantMap& map, QObject* parent) :
    Named(map.value(props::id, utils::generateId()), map.value(props::name).toString(), parent)
{
}

QString Named::name() const
{
    return m_name;
}

QVariantMap Named::toVariantMap() const
{
    QVariantMap map = Entity::toVariantMap();
    map.insert(props::name, m_name);
    return map;
}

void Named::fromVariantMap(const QVariantMap& map)
{
    m_name = map.value(props::name, m_name).toString();
    Entity::fromVariantMap(map);

    emit changed();
}

void Named::setName(const QString& name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit changed();
}
