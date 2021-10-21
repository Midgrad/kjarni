#include "waypoint.h"

#include "route.h"

using namespace md::domain;

Waypoint::Waypoint(const QString& name, const WaypointType* type, QObject* parent) :
    Entity(name, parent),
    m_type(type)
{
    Q_ASSERT(type);
}

Waypoint::Waypoint(const QVariantMap& map, const WaypointType* type, QObject* parent) :
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

    if (m_route)
        map.insert(params::route, m_route->id());

    return map;
}

const WaypointType* Waypoint::type() const
{
    return m_type;
}

Route* Waypoint::route() const
{
    return m_route;
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

void Waypoint::setRoute(Route* route)
{
    if (m_route == route)
        return;

    m_route = route;
    emit routeChanged(route);
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
