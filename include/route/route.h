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

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    const RouteType* type() const;

    int count() const;
    int index(Waypoint* waypoint) const;
    const QList<Waypoint*>& waypoints() const;
    Waypoint* waypoint(int index) const;

public slots:
    void setWaypoints(const QList<Waypoint*>& waypoins);
    void addWaypoint(Waypoint* waypoint);
    void removeWaypoint(Waypoint* waypoint);

signals:
    void waypointAdded(int index, Waypoint* waypoint);
    void waypointChanged(int index, Waypoint* waypoint);
    void waypointRemoved(int index, Waypoint* waypoint);

private:
    void fromVariantMapImpl(const QVariantMap& map);

    const RouteType* const m_type;
    QList<Waypoint*> m_waypoins;
};
} // namespace md::domain

#endif // ROUTE_H
