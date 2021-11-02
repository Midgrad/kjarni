#include "mission_route.h"

#include <QDebug>
#include <QMetaEnum>

#include "mission_traits.h"

using namespace md::domain;

MissionRoute::MissionRoute(Route* route, QObject* parent) : QObject(parent), m_route(route)
{
    for (int index = 0; index < route->count(); ++index)
    {
        m_waypointStatuses[index] = MissionWaypoint();
    }
}

Route* MissionRoute::route() const
{
    return m_route;
}

MissionWaypoint MissionRoute::waypointStatus(int index) const
{
    return m_waypointStatuses.value(index, MissionWaypoint());
}

Waypoint* MissionRoute::waypoint(int index) const
{
    return m_route->waypoint(index);
}

int MissionRoute::currentWaypoint() const
{
    return m_currentWaypoint;
}

int MissionRoute::count() const
{
    return m_route->count();
}

QVariantList MissionRoute::toVariantList() const
{
    QVariantList list;
    for (const MissionWaypoint& wpt : m_waypointStatuses)
    {
        list.append(wpt.toVariantMap());
    }
    return list;
}

void MissionRoute::setWaypointStatus(int index, MissionWaypoint status)
{
    if (this->waypointStatus(index) == status)
        return;

    m_waypointStatuses[index] = status;
    emit waypointStatusChanged(index, status);
}

void MissionRoute::setCurrentWaypoint(int index)
{
    if (m_currentWaypoint == index)
        return;

    if (m_currentWaypoint > -1)
    {
        this->setWaypointStatus(index, MissionWaypoint::Normal);
    }

    m_currentWaypoint = index;

    if (m_currentWaypoint > -1)
    {
        this->setWaypointStatus(index, MissionWaypoint::Current);
    }
    emit currentWaypointChanged(index);
}

void MissionRoute::addWaypoint(Waypoint* waypoint)
{
    m_route->addWaypoint(waypoint);
}
