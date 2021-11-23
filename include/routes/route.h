#ifndef ROUTE_H
#define ROUTE_H

#include "route_type.h"
#include "waypoint_item.h"

namespace md::domain
{
class Route : public Named
{
    Q_OBJECT

public:
    Route(const RouteType* type, const QString& name, const QVariant& id = utils::generateId(),
          QObject* parent = nullptr);
    Route(const RouteType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    const RouteType* type() const;

    int waypointsCount() const;
    int waypointIndex(WaypointItem* waypoint) const;
    const QList<WaypointItem*>& waypoints() const;
    WaypointItem* waypoint(int index) const;

    int itemsCount() const;
    QList<WaypointItem*> items() const;
    WaypointItem* item(int index) const;

public slots:
    void setWaypoints(const QList<WaypointItem*>& waypoints);
    void addWaypoint(WaypointItem* waypoint);
    void removeWaypoint(WaypointItem* waypoint);
    void clear();

signals:
    void waypointAdded(int index, WaypointItem* waypoint);
    void waypointChanged(int index, WaypointItem* waypoint);
    void waypointRemoved(int index, WaypointItem* waypoint);

private:
    void fromVariantMapImpl(const QVariantMap& map);

    const RouteType* const m_type;
    QList<WaypointItem*> m_waypoints;
};
} // namespace md::domain

#endif // ROUTE_H
