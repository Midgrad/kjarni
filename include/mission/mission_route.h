#ifndef MISSION_ROUTE_H
#define MISSION_ROUTE_H

#include "mission_type.h"
#include "route.h"

namespace md::domain
{
// TODO: MissionRouteItem
class MissionRoute : public Entity
{
    Q_OBJECT

public:
    MissionRoute(const MissionType* type, const QVariant& id, const QString& name,
                 QObject* parent = nullptr);
    MissionRoute(const MissionType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;

    Waypoint* homePoint() const;
    Route* route() const;
    int currentItem() const;

    int count();
    WaypointItem* item(int index) const;
    QList<WaypointItem*> items() const;

public slots:
    void assignRoute(Route* route);
    void setCurrentItem(int currentItem);

signals:
    void routeChanged(Route* route);
    void itemsChanged();
    void currentItemChanged(int index);

    void switchCurrentItem(int item); // Goto item

private:
    Waypoint* const m_homePoint;
    Route* m_route = nullptr;
    int m_currentItem = -1;
};
} // namespace md::domain

#endif // MISSION_ROUTE_H
