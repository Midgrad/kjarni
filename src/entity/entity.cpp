#include "entity.h"

using namespace md::domain;

Entity::Entity(const QVariant& id, const QString& name, const QVariantMap& parameters,
               QObject* parent) :
    QObject(parent),
    m_id(id),
    m_name(name),
    m_parameters(parameters)
{
}

Entity::Entity(const QVariantMap& map, QObject* parent) :
    Entity(map.value(props::id, utils::generateId()), map.value(props::name).toString(),
           map.value(props::params).toMap(), parent)
{
}

Entity::~Entity()
{
}

QVariant Entity::id() const
{
    return m_id;
}

QString Entity::name() const
{
    return m_name;
}

const QVariantMap& Entity::parameters() const
{
    return m_parameters;
}

QVariant Entity::parameter(const QString& key) const
{
    return m_parameters.value(key);
}

QVariantMap Entity::toVariantMap() const
{
    QVariantMap map;

    if (!m_id.isNull())
        map.insert(props::id, m_id.toString());
    if (!m_name.isNull())
        map.insert(props::name, m_name);
    if (!m_parameters.isEmpty())
        map.insert(props::params, m_parameters);

    return map;
}

void Entity::fromVariantMap(const QVariantMap& map)
{
    m_name = map.value(props::name, m_name).toString();
    m_parameters = map.value(props::params, m_parameters).toMap();

    emit changed();
}

void Entity::setParameters(const QVariantMap& parameters)
{
    if (m_parameters == parameters)
        return;

    m_parameters = parameters;
    emit changed();
}

void Entity::setParameter(const QString& key, const QVariant& value)
{
    if (m_parameters.value(key) == value)
        return;

    m_parameters[key] = value;
    emit changed();
}

void Entity::removeParameters(const QStringList& keys)
{
    bool changedFlag = false;

    for (const QString& key : keys)
    {
        if (m_parameters.remove(key))
        {
            changedFlag = true;
        }
    }

    if (changedFlag)
        emit changed();
}

void Entity::removeParameter(const QString& key)
{
    if (m_parameters.remove(key))
    {
        emit changed();
    }
}

void Entity::setName(const QString& name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit changed();
}
