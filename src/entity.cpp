#include "entity.h"

using namespace md::domain;

Entity::Entity(QObject* parent) : QObject(parent)
{
}

Entity::~Entity()
{
}

QVariantMap Entity::parameters() const
{
    return m_parameters;
}

QVariant Entity::parameter(const QString& key) const
{
    return m_parameters.value(key);
}

void Entity::setParameters(const QVariantMap& parameters)
{
    if (m_parameters == parameters)
        return;

    m_parameters = parameters;

    for (auto it = parameters.constBegin(); it != parameters.constEnd(); ++it)
        emit parameterChanged(it.key(), it.value());
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

void Entity::removeParameter(const QString& key)
{
    if (m_parameters.remove(key))
    {
        emit parameterChanged(key, QVariant());
        emit parametersChanged();
    }
}
