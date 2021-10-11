#include "waypoint.h"

using namespace md::domain;

Waypoint::Waypoint(const QString& name, const WaypointType* type, QObject* parent) :
    Entity(name, parent),
    m_type(type)
{
    Q_ASSERT(type);
}

Waypoint::Waypoint(const QJsonObject& json, const WaypointType* type, QObject* parent) :
    Entity(json, parent),
    m_type(type)
{
    Q_ASSERT(type);
}

QJsonObject Waypoint::toJson(bool recursive) const
{
    Q_UNUSED(recursive)

    QJsonObject json = Entity::toJson();

    json.insert(params::type, m_type->name);

    return json;
}

const WaypointType* Waypoint::type() const
{
    return m_type;
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

void Waypoint::setType(const WaypointType* type)
{
    Q_ASSERT(type);

    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged();

    this->syncParameters();
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
