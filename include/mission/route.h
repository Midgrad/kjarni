#ifndef ROUTE_H
#define ROUTE_H

#include "route_type.h"
#include "waypoint.h"

namespace md::domain
{
class Route : public Entity
{
    Q_OBJECT

public:
    Route(const RouteType* routeType, QObject* parent = nullptr);
    Route(const QJsonObject& json, const RouteType* routeType, QObject* parent = nullptr);

    QJsonObject toJson(bool recursive) const override;
    void fromJson(const QJsonObject& json) override;

    int count() const;
    const QList<Waypoint*>& waypoints() const;
    Waypoint* waypoint(int index) const;

public slots:
    void addWaypoint(Waypoint* waypoint);
    void removeWaypoint(Waypoint* waypoint);

signals:
    void waypointAdded(Waypoint* waypoint);
    void waypointRemoved(Waypoint* waypoint);

private:
    void fromJsonImpl(const QJsonObject& json);

    const RouteType* const m_routeType;
    QList<Waypoint*> m_waypoins;
};
} // namespace md::domain

#endif // ROUTE_H
