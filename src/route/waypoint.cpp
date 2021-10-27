#include "waypoint.h"

using namespace md::domain;

Waypoint::Waypoint(const WaypointType* type, const QString& name, const QVariant& id,
                   const QVariantMap& parameters, QObject* parent) :
    Entity(id, name, parameters, parent),
    m_type(type)
{
    Q_ASSERT(type);
}

Waypoint::Waypoint(const WaypointType* type, const QString& name, const QVariant& id,
                   QObject* parent) :
    Waypoint(type, name, id, type->defaultParameters(), parent)
{
}

Waypoint::Waypoint(const WaypointType* type, const QVariantMap& map, QObject* parent) :
    Entity(map, parent),
    m_type(type)
{
    Q_ASSERT(type);
}

QVariantMap Waypoint::toVariantMap(bool recursive) const
{
    Q_UNUSED(recursive)

    QVariantMap map = Entity::toVariantMap();
    map.insert(params::type, m_type->name);

    return map;
}

const WaypointType* Waypoint::type() const
{
    return m_type;
}

void Waypoint::setType(const WaypointType* type)
{
    Q_ASSERT(type);

    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged();

    this->syncParameters();
}

void Waypoint::setAndCheckParameter(const QString& key, const QVariant& value)
{
    QVariant guarded = value;
    auto parameter = m_type->parameter(key);
    if (parameter)
    {
        guarded = parameter->guard(value);
    }
    this->setParameter(key, guarded);
}

void Waypoint::resetParameter(const QString& key)
{
    auto parameter = m_type->parameter(key);
    if (!parameter)
        return;

    this->setParameter(key, parameter->defaultValue);
}

void Waypoint::resetParameters()
{
    this->setParameters(m_type->defaultParameters());
}

void Waypoint::syncParameters()
{
    QStringList unneededParameters = this->parameters().keys();
    for (const Parameter* parameter : m_type->parameters)
    {
        // If parameter exist - remove it from unneededParameters
        if (!unneededParameters.removeOne(parameter->name))
        {
            // Or add it with default value
            this->setParameter(parameter->name, parameter->defaultValue);
        }
    }
    this->removeParameters(unneededParameters);
}
