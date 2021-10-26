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
    Route(const QString& name, const RouteType* type, QObject* parent = nullptr);
    Route(const QVariantMap& map, const RouteType* type, QObject* parent = nullptr);

    QVariantMap toVariantMap(bool recursive) const override;
    void fromVariantMap(const QVariantMap& map) override;

    const RouteType* type() const;

    int count() const;
    const QList<Waypoint*>& waypoints() const;
    Waypoint* waypoint(int index) const;

public slots:
    void setWaypoints(const QList<Waypoint*>& waypoins);
    void addWaypoint(Waypoint* waypoint);
    void removeWaypoint(Waypoint* waypoint);

signals:
    void waypointAdded(Waypoint* waypoint);
    void waypointChanged(Waypoint* waypoint);
    void waypointRemoved(Waypoint* waypoint);

private:
    void fromVariantMapImpl(const QVariantMap& map);

    const RouteType* const m_type;
    QList<Waypoint*> m_waypoins;
};
} // namespace md::domain

#endif // ROUTE_H
