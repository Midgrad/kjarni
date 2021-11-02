#ifndef MISSION_ROUTE_H
#define MISSION_ROUTE_H

#include "mission_waypoint.h"
#include "route.h"

namespace md::domain
{
class MissionRoute : public QObject
{
    Q_OBJECT

public:
    MissionRoute(Route* route, QObject* parent = nullptr);

    Route* route() const;
    MissionWaypoint waypointStatus(int index) const;
    Waypoint* waypoint(int index) const;
    int currentWaypoint() const;
    int count() const;

    QVariantList toVariantList() const;

public slots:
    void setWaypointStatus(int index, MissionWaypoint status);
    void setCurrentWaypoint(int index);
    void addWaypoint(Waypoint* waypoint);

signals:
    void waypointStatusChanged(int index, MissionWaypoint status);
    void currentWaypointChanged(int index);

    void switchWaypoint(int waypoint); // Goto waypoint

private:
    Route* const m_route; // NOTE: later maybe
    QMap<int, MissionWaypoint> m_waypointStatuses;
    int m_currentWaypoint = -1;
};

} // namespace md::domain

#endif // MISSION_ROUTE_H
