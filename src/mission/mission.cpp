#include "mission.h"

#include <QDebug>

using namespace md::domain;

Mission::Mission(const MissionType& type, const QVariant& id, const QString& name,
                 const QString& vehicle, QObject* parent) :
    Entity(id, name, parent),
    m_type(type),
    m_route(new Route(type.routeType, this)),
    m_vehicle(vehicle)
{
}

Mission::Mission(const QJsonObject& json, const MissionType& type, QObject* parent) :
    Entity(json, parent),
    m_type(type),
    m_vehicle(json.value(params::vehicle).toString()),
    m_route(new Route(json.value(params::route).toObject(), type.routeType, this))
{
}

QJsonObject Mission::toJson(bool recursive) const
{
    QJsonObject json = Entity::toJson();

    json.insert(params::type, m_type.name);
    json.insert(params::vehicle, m_vehicle);
    if (recursive)
        json.insert(params::route, m_route->toJson(recursive));

    return json;
}

void Mission::fromJson(const QJsonObject& json)
{
    if (json.contains(params::route))
        m_route->fromJson(json.value(params::route).toObject());

    Entity::fromJson(json);
}

const MissionType& Mission::type() const
{
    return m_type;
}

QString Mission::vehicle() const
{
    return m_vehicle;
}

Route* Mission::route() const
{
    return m_route;
}

const MissionStatus& Mission::status() const
{
    return m_status;
}

int Mission::currentWaypoint() const
{
    return m_currentWaypoint;
}

void Mission::updateStatus(MissionStatus::Type type, int progress, int total)
{
    progress = progress == -1 ? m_status.progress() : progress;
    total = total == -1 ? m_status.total() : total;

    MissionStatus status(type, progress, total);
    if (m_status == status)
        return;

    m_status = status;
    emit statusChanged(status);
}

void Mission::updateStatusProgress(int progress)
{
    if (progress >= m_status.total())
    {
        this->updateStatus(MissionStatus::Actual);
    }
    else
    {
        this->updateStatus(m_status.type(), progress);
    }
}

void Mission::setCurrentWaypoint(int currentWaypoint)
{
    if (m_currentWaypoint == currentWaypoint)
        return;

    m_currentWaypoint = currentWaypoint;
    emit currentWaypointChanged(currentWaypoint);
}
