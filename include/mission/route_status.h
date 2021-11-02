#ifndef ROUTE_STATUS_H
#define ROUTE_STATUS_H

#include "waypoint_status.h"

namespace md::domain
{
class RouteStatus : public QObject
{
    Q_OBJECT

public:
    RouteStatus(QObject* parent = nullptr);

    WaypointStatus waypointStatus(int index) const;

    QVariantList toVariantList() const;

public slots:
    void setWaypointStatus(int index, WaypointStatus status);

signals:
    void waypointStatusChanged(int index, WaypointStatus status);

private:
    QMap<int, WaypointStatus> m_waypointStatuses;
};

} // namespace md::domain

#endif // ROUTE_STATUS_H
