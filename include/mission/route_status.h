#ifndef ROUTE_STATUS_H
#define ROUTE_STATUS_H

#include "route.h"
#include "waypoint_status.h"

namespace md::domain
{
class RouteStatus : public QObject // TODO: MissionRoute
{
    Q_OBJECT

public:
    RouteStatus(Route* route, QObject* parent = nullptr);

    WaypointStatus waypointStatus(int index) const;
    int currentWaypoint() const;

    QVariantList toVariantList() const;

public slots:
    void setWaypointStatus(int index, WaypointStatus status);
    void setCurrentWaypoint(int index);

signals:
    void waypointStatusChanged(int index, WaypointStatus status);
    void currentWaypointChanged(int index);

    void switchWaypoint(int waypoint); // Goto waypoint

private:
    const Route* m_route;
    QMap<int, WaypointStatus> m_waypointStatuses;
    int m_currentWaypoint = -1;
};

} // namespace md::domain

#endif // ROUTE_STATUS_H
