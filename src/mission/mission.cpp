#include "mission.h"

#include <QDebug>

using namespace md::domain;

Mission::Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
                 const QVariant& id, QObject* parent) :
    Entity(id, name, QVariantMap(), parent),
    m_type(type),
    m_vehicleId(vehicleId)
{
}

Mission::Mission(const MissionType* type, const QVariantMap& map, QObject* parent) :
    Entity(map, parent),
    m_type(type),
    m_vehicleId(map.value(params::vehicle))
{
    this->fromVariantMap(map);
}

QVariantMap Mission::toVariantMap(bool recursive) const
{
    QVariantMap map = Entity::toVariantMap();

    map.insert(params::type, m_type->name);
    map.insert(params::vehicle, m_vehicleId);

    if (m_route)
    {
        if (recursive)
            map.insert(params::route, m_route->toVariantMap(recursive));
        else
            map.insert(params::route, m_route->id());
    }

    return map;
}

void Mission::fromVariantMap(const QVariantMap& map)
{
    if (map.contains(params::route) && m_route)
    {
        m_route->fromVariantMap(map.value(params::route).toMap());
    }

    Entity::fromVariantMap(map);
}

const MissionType* Mission::type() const
{
    return m_type;
}

QVariant Mission::vehicleId() const
{
    return m_vehicleId;
}

Route* Mission::route() const
{
    return m_route;
}

RouteStatus* Mission::routeStatus() const
{
    return m_routeStatus;
}

const MissionStatus& Mission::missionStatus() const
{
    return m_status;
}

void Mission::setRoute(Route* route)
{
    if (m_route == route)
        return;

    if (m_routeStatus)
        m_routeStatus->deleteLater();

    m_route = route;
    m_routeStatus = m_route ? new RouteStatus(m_route, this) : nullptr;

    emit routeChanged(m_route, m_routeStatus);
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
