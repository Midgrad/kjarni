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

Entity::Entity(const QJsonObject& object, QObject* parent) :
    Entity(object.value(params::id), object.value(params::name).toString(),
           object.value(params::params).toVariant().toMap(), parent)
{
}

Entity::Entity(const QVariant& id, const QString& name, QObject* parent) :
    Entity(id, name, QVariantMap(), parent)
{
}

Entity::Entity(const QString& name, QObject* parent) : Entity(QVariant(), name, parent)
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

QJsonObject Entity::toJson() const
{
    QJsonObject json;

    json.insert(params::id, QJsonValue::fromVariant(m_id));
    json.insert(params::name, m_name);
    json.insert(params::params, QJsonValue::fromVariant(m_parameters));

    return json;
}

void Entity::setParameters(const QVariantMap& parameters)
{
    if (m_parameters == parameters)
        return;

    m_parameters = parameters;

    for (auto it = parameters.constBegin(); it != parameters.constEnd(); ++it)
    {
        emit parameterChanged(it.key(), it.value());
    }
    emit parametersChanged();
}

void Entity::setParameter(const QString& key, const QVariant& value)
{
    if (m_parameters.value(key) == value)
        return;

    m_parameters[key] = value;
    emit parameterChanged(key, value);
    emit parametersChanged();
}

void Entity::removeParameters(const QStringList& keys)
{
    bool changed = false;

    for (const QString& key : keys)
    {
        if (m_parameters.remove(key))
        {
            changed = true;
            emit parameterChanged(key, QVariant());
        }
    }

    if (changed)
        emit parametersChanged();
}

void Entity::removeParameter(const QString& key)
{
    if (m_parameters.remove(key))
    {
        emit parameterChanged(key, QVariant());
        emit parametersChanged();
    }
}

void Entity::setName(const QString& name)
{
    if (m_name == name)
        return;
    m_name = name;
    emit nameChanged();
}
