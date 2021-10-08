#include "waypoint.h"

using namespace md::domain;

Waypoint::Waypoint(const QString& name, WaypointType* type, QObject* parent) :
    Entity(name, parent),
    m_type(type)
{
}

Waypoint::Waypoint(const QJsonObject& json, WaypointType* type, QObject* parent) :
    Entity(json, parent),
    m_type(type)
{
}

QJsonObject Waypoint::toJson(bool recursive) const
{
    Q_UNUSED(recursive)

    QJsonObject json = Entity::toJson();

    json.insert(params::type, m_type->name);

    return json;
}

WaypointType* Waypoint::type() const
{
    return m_type;
}

void Waypoint::setType(WaypointType* type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged();
}
