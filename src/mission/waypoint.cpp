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

void Waypoint::setAndCheckParameter(const QString& parameter, const QVariant& value)
{
    // TODO: check parameter.guard(m_waypointFillers[parameter.name](item));
    this->setParameter(parameter, value);
}

void Waypoint::setType(const WaypointType* type)
{
    Q_ASSERT(type);

    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged();
}
