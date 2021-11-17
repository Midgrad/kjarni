#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "geodetic.h"
#include "route_item.h"
#include "waypoint_type.h"

namespace md::domain
{
class Waypoint : public RouteItem
{
    Q_OBJECT

public:
    Waypoint(const WaypointType* type, const QVariant& id = utils::generateId(),
             QObject* parent = nullptr);
    Waypoint(const WaypointType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    const WaypointType* type() const override;

    QVariantMap calcData();
    const Geodetic& position() const;

    int count() const;
    int index(RouteItem* item) const;
    RouteItem* item(int index) const;
    const QList<RouteItem*>& items() const;

    bool current() const;
    bool reached() const;
    bool confirmed() const;

public slots:
    void setType(const WaypointType* type);

    void setCalcData(const QVariantMap& calcData);
    void setPosition(const Geodetic& position);

    void setItems(const QList<RouteItem*>& items);
    void addItem(RouteItem* item);
    void removeItem(RouteItem* item);

    void setCurrent(bool current);
    void setReached(bool reached);
    void setConfirmed(bool confirmed);

signals:
    void itemAdded(int index, RouteItem* item);
    void itemChanged(int index, RouteItem* item);
    void itemRemoved(int index, RouteItem* item);

private:
    const WaypointType* m_type;
    QVariantMap m_calcData;
    Geodetic m_position;
    QList<RouteItem*> m_items;

    // TODO: to MissionRoute
    bool m_current = false;
    bool m_reached = false;
    bool m_confirmed = false;
};
} // namespace md::domain

#endif // WAYPOINT_H
