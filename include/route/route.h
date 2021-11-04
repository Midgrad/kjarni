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
    Route(const RouteType* type, const QString& name, const QVariant& id = utils::generateId(),
          QObject* parent = nullptr);
    Route(const RouteType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap(bool recursive) const override;
    void fromVariantMap(const QVariantMap& map) override;

    const RouteType* type() const;

    int count() const;
    int index(Waypoint* waypoint) const;
    const QList<Waypoint*>& waypoints() const;
    Waypoint* waypoint(int index) const;
    Waypoint* currentWaypoint() const;
    int currentWaypointIndex() const;

public slots:
    void setWaypoints(const QList<Waypoint*>& waypoins);
    void addWaypoint(Waypoint* waypoint);
    void removeWaypoint(Waypoint* waypoint);
    void setCurrentWaypointIndex(int currentWaypointIndex);

signals:
    void waypointAdded(Waypoint* waypoint);
    void waypointChanged(Waypoint* waypoint);
    void waypointRemoved(Waypoint* waypoint);
    void currentWaypointChanged(int index);

    void switchWaypoint(int waypoint); // Goto waypoint

private:
    void fromVariantMapImpl(const QVariantMap& map);

    const RouteType* const m_type;
    QList<Waypoint*> m_waypoins;
    Waypoint* m_currentWaypoint = nullptr;
};
} // namespace md::domain

#endif // ROUTE_H
