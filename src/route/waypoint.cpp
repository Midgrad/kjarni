#include "waypoint.h"

#include <QDebug>
#include <QMetaEnum>

using namespace md::domain;

namespace
{
Waypoint::State stateFromVariant(const QVariant& value)
{
    auto&& metaEnum = QMetaEnum::fromType<Waypoint::State>();
    return static_cast<Waypoint::State>(metaEnum.keyToValue(value.toString().toUtf8().constData()));
}
} // namespace

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
    m_state(::stateFromVariant(map.value(params::state)))
{
    Q_ASSERT(type);
}

QVariantMap Waypoint::toVariantMap(bool recursive) const
{
    Q_UNUSED(recursive)

    QVariantMap map = Entity::toVariantMap();
    map.insert(params::type, m_type->name);
    map.insert(params::state, QVariant::fromValue(m_state).toString());

    return map;
}

void Waypoint::fromVariantMap(const QVariantMap& map)
{
    if (map.contains(params::state))
        this->setState(::stateFromVariant(map.value(params::state)));

    Entity::fromVariantMap(map);
}

const WaypointType* Waypoint::type() const
{
    return m_type;
}

Waypoint::State Waypoint::state() const
{
    return m_state;
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

void Waypoint::setState(State state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit stateChanged();
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
