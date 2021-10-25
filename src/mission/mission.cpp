#include "mission.h"

#include <QDebug>

using namespace md::domain;

Mission::Mission(const MissionType* type, const QString& name, const QUuid& vehicleId,
                 QObject* parent) :
    Entity(name, parent),
    m_type(type),
    m_route(new Route(name + " " + tr("Route"), type->routeType, this)),
    m_vehicleId(vehicleId)
{
}

Mission::Mission(const QVariantMap& map, const MissionType* type, QObject* parent) :
    Entity(map, parent),
    m_type(type),
    m_vehicleId(map.value(params::vehicle).toString()),
    m_route(new Route(map.value(params::route).toMap(), type->routeType, this))
{
}

QVariantMap Mission::toVariantMap(bool recursive) const
{
    QVariantMap map = Entity::toVariantMap();

    map.insert(params::type, m_type->name);
    map.insert(params::vehicle, m_vehicleId);

    if (recursive)
        map.insert(params::route, m_route->toVariantMap(recursive));
    else
        map.insert(params::route, m_route->id());

    return map;
}

void Mission::fromVariantMap(const QVariantMap& map)
{
    if (map.contains(params::route))
        m_route->fromVariantMap(map.value(params::route).toMap());

    Entity::fromVariantMap(map);
}

const MissionType* Mission::type() const
{
    return m_type;
}

QUuid Mission::vehicleId() const
{
    return m_vehicleId;
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