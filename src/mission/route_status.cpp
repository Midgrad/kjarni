#include "route_status.h"

#include <QDebug>
#include <QMetaEnum>

#include "mission_traits.h"

using namespace md::domain;

RouteStatus::RouteStatus(Route* route, QObject* parent) : QObject(parent), m_route(route)
{
    for (int index = 0; index < route->count(); ++index)
    {
        m_waypointStatuses[index] = WaypointStatus();
    }
}

WaypointStatus RouteStatus::waypointStatus(int index) const
{
    return m_waypointStatuses.value(index, WaypointStatus());
}

int RouteStatus::currentWaypoint() const
{
    return m_currentWaypoint;
}

QVariantList RouteStatus::toVariantList() const
{
    QVariantList list;
    for (const WaypointStatus& wpt : m_waypointStatuses)
    {
        list.append(wpt.toVariantMap());
    }
    return list;
}

void RouteStatus::setWaypointStatus(int index, WaypointStatus status)
{
    if (this->waypointStatus(index) == status)
        return;

    m_waypointStatuses[index] = status;
    emit waypointStatusChanged(index, status);
}

void RouteStatus::setCurrentWaypoint(int index)
{
    if (m_currentWaypoint == index)
        return;

    if (m_currentWaypoint > -1)
    {
        this->setWaypointStatus(index, WaypointStatus::Normal);
    }

    m_currentWaypoint = index;

    if (m_currentWaypoint > -1)
    {
        this->setWaypointStatus(index, WaypointStatus::Current);
    }
    emit currentWaypointChanged(index);
}
