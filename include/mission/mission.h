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
    // TODO: to MissionRoute
    Waypoint* homePoint() const;
    Route* route() const;
    int currentItem() const;

    int count();
    WaypointItem* item(int index) const;
    QList<WaypointItem*> items() const;

    //    int waypointsCount() const;
    //    QList<Waypoint*> waypoints() const;
    //    Waypoint* waypoint(int index) const;
    //    int itemsCount() const;
    //    QList<WaypointItem*> items() const;

public slots:
    void assignRoute(Route* route);
    void setCurrentItem(int currentItem);

signals:
    void routeChanged(Route* route);
    void itemsChanged();
    void currentItemChanged(int index);

    void switchCurrentItem(int item); // Goto item

private:
    const MissionType* const m_type;
    const QVariant m_vehicleId;
    MissionOperation* const m_operation; //TODO: mision operation to service
    Waypoint* const m_homePoint;
    Route* m_route = nullptr;
    int m_currentItem = -1;
};
} // namespace md::domain

#endif // MISSION_H
