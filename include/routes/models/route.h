#ifndef ROUTE_H
#define ROUTE_H

#include "route_type.h"
#include "route_item.h"

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
    int waypointIndex(RouteItem* waypoint) const;
    const QList<RouteItem*>& waypoints() const;
    RouteItem* waypoint(int index) const;

    int itemsCount() const;
    QList<RouteItem*> items() const;
    RouteItem* item(int index) const;

public slots:
    void setWaypoints(const QList<RouteItem*>& waypoints);
    void addWaypoint(RouteItem* waypoint);
    void removeWaypoint(RouteItem* waypoint);
    void clear();

signals:
    void waypointAdded(int index, RouteItem* waypoint);
    void waypointChanged(int index, RouteItem* waypoint);
    void waypointRemoved(int index, RouteItem* waypoint);

private:
    void fromVariantMapImpl(const QVariantMap& map);

    const RouteType* const m_type;
    QList<RouteItem*> m_waypoints;
};
} // namespace md::domain

#endif // ROUTE_H
