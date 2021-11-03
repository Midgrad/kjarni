#include "waypoint.h"

#include <QDebug>

#include "route_traits.h"

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
    m_type(type),
    m_current(map.value(params::current, false).toBool()),
    m_reached(map.value(params::reached, false).toBool()),
    m_confirmed(map.value(params::confirmed, false).toBool())
{
    Q_ASSERT(type);
}

QVariantMap Waypoint::toVariantMap(bool recursive) const
{
    Q_UNUSED(recursive)

    QVariantMap map = Entity::toVariantMap();

    map.insert(params::type, m_type->name);
    map.insert(params::current, m_current);
    map.insert(params::reached, m_reached);
    map.insert(params::confirmed, m_confirmed);

    return map;
}

void Waypoint::fromVariantMap(const QVariantMap& map)
{
    m_current = map.value(params::current, m_current).toBool();
    m_reached = map.value(params::reached, m_reached).toBool();
    m_confirmed = map.value(params::confirmed, m_confirmed).toBool();

    Entity::fromVariantMap(map);
}

const WaypointType* Waypoint::type() const
{
    return m_type;
}

bool Waypoint::current() const
{
    return m_current;
}

bool Waypoint::reached() const
{
    return m_reached;
}

bool Waypoint::confirmed() const
{
    return m_confirmed;
}

void Waypoint::setType(const WaypointType* type)
{
    Q_ASSERT(type);

    if (m_type == type)
        return;

    m_type = type;
    emit changed();

    this->syncParameters();
}

void Waypoint::setCurrent(bool current)
{
    if (m_current == current)
        return;

    m_current = current;
    emit changed();
}

void Waypoint::setConfirmed(bool confirmed)
{
    if (m_confirmed == confirmed)
        return;

    m_confirmed = confirmed;
    emit changed();
}

void Waypoint::setReached(bool reached)
{
    if (m_reached == reached)
        return;

    m_reached = reached;
    emit changed();
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
    QVariantMap parameters = this->parameters();

    // Add parameters defaulted by type
    for (const Parameter* parameter : m_type->parameters)
    {
        if (!parameters.contains(parameter->name))
            parameters.insert(parameter->name, parameter->defaultValue);
    }

    // Remove unneeded parameters
    for (const QString& key : parameters.keys())
    {
        if (!m_type->parameter(key))
            parameters.remove(key);
    }

    this->setParameters(parameters);
}
