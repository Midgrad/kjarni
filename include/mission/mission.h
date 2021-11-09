#ifndef MISSION_H
#define MISSION_H

#include "mission_operation.h"
#include "mission_type.h"
#include "route.h"

namespace md::domain
{
class Mission : public Entity
{
    Q_OBJECT

public:
    Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
            const QVariant& id = utils::generateId(), QObject* parent = nullptr);
    Mission(const MissionType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;

    const MissionType* type() const;
    QVariant vehicleId() const;
    MissionOperation* operation() const;
    RouteItem* homePoint() const;
    Route* route() const;
    int count() const;
    RouteItem* waypoint(int index) const;
    QList<RouteItem*> waypoints() const;
    RouteItem* currentWaypoint() const;
    int currentWaypointIndex() const;

public slots:
    void assignRoute(Route* route);
    void setCurrentWaypointIndex(int currentWaypointIndex);

signals:
    void routeChanged(Route* route);
    void waypointsChanged();
    void currentWaypointChanged(int index);

    void switchWaypoint(int waypoint); // Goto waypoint

private:
    const MissionType* m_type;
    const QVariant m_vehicleId;
    MissionOperation* const m_operation; //TODO: mision operation to service
    RouteItem* const m_homePoint;
    RouteItem* m_currentWaypoint = nullptr;
    Route* m_route = nullptr;
};
} // namespace md::domain

#endif // MISSION_H
