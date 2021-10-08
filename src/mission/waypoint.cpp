#include "waypoint.h"

using namespace md::domain;

Waypoint::Waypoint(const QString& name, const QString& type, QObject* parent) :
    Entity(name, parent),
    m_type(type)
{
}

Waypoint::Waypoint(const QJsonObject& json, QObject* parent) :
    Entity(json, parent),
    m_type(json.value(params::type).toString())
{
}

QJsonObject Waypoint::toJson(bool recursive) const
{
    Q_UNUSED(recursive)

    QJsonObject json = Entity::toJson();

    json.insert(params::type, m_type);

    return json;
}

void Waypoint::fromJson(const QJsonObject& json)
{
    if (json.contains(params::type))
        this->setType(json.value(params::type).toString());

    Entity::fromJson(json);
}

QString Waypoint::type() const
{
    return m_type;
}

void Waypoint::setType(const QString& type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged();
}
