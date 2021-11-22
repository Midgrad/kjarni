#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "geodetic.h"
#include "waypoint_item.h"
#include "waypoint_item_type.h"

namespace md::domain
{
class Waypoint : public WaypointItem
{
    Q_OBJECT

public:
    Waypoint(const WaypointItemType* type, const QVariant& id = utils::generateId(),
             QObject* parent = nullptr);
    Waypoint(const WaypointItemType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    const WaypointItemType* type() const override;

    QVariantMap calcData();
    const Geodetic& position() const;

    int count() const;
    int index(WaypointItem* item) const;
    WaypointItem* item(int index) const;
    const QList<WaypointItem*>& items() const;

public slots:
    void setType(const WaypointItemType* type);

    void setCalcData(const QVariantMap& calcData);
    void setPosition(const Geodetic& position);

    void setItems(const QList<WaypointItem*>& items);
    void addItem(WaypointItem* item);
    void removeItem(WaypointItem* item);

signals:
    void itemAdded(int index, WaypointItem* item);
    void itemChanged(int index, WaypointItem* item);
    void itemRemoved(int index, WaypointItem* item);

private:
    const WaypointItemType* m_type;
    QVariantMap m_calcData;
    Geodetic m_position;
    QList<WaypointItem*> m_items;
};
} // namespace md::domain

#endif // WAYPOINT_H
