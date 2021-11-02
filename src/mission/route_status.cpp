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
