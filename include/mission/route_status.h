#ifndef ROUTE_STATUS_H
#define ROUTE_STATUS_H

#include "route.h"
#include "waypoint_status.h"

namespace md::domain
{
class RouteStatus : public QObject
{
    Q_OBJECT

public:
    RouteStatus(Route* route, QObject* parent = nullptr);

    WaypointStatus waypointStatus(int index) const;

    QVariantList toVariantList() const;

public slots:
    void setWaypointStatus(int index, WaypointStatus status);

signals:
    void waypointStatusChanged(int index, WaypointStatus status);

private:
    const Route* m_route;
    QMap<int, WaypointStatus> m_waypointStatuses;
};

} // namespace md::domain

#endif // ROUTE_STATUS_H
