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
    int index(RouteItem* waypoint) const;
    const QList<RouteItem*>& waypoints() const;
    RouteItem* waypoint(int index) const;

public slots:
    void setWaypoints(const QList<RouteItem*>& waypoins);
    void addWaypoint(RouteItem* waypoint);
    void removeWaypoint(RouteItem* waypoint);

signals:
    void waypointAdded(int index, RouteItem* waypoint);
    void waypointChanged(int index, RouteItem* waypoint);
    void waypointRemoved(int index, RouteItem* waypoint);

private:
    void fromVariantMapImpl(const QVariantMap& map);

    const RouteType* const m_type;
    QList<RouteItem*> m_waypoins;
};
} // namespace md::domain

#endif // ROUTE_H
