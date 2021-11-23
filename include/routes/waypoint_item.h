#ifndef WAYPOINT_ITEM_H
#define WAYPOINT_ITEM_H

#include "entity.h"
#include "waypoint_item_type.h"

namespace md::domain
{
class WaypointItem : public Parametrised
{
    Q_OBJECT

public:
    WaypointItem(const WaypointItemType* type, const QVariant& id = utils::generateId(),
                 QObject* parent = nullptr);
    WaypointItem(const WaypointItemType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    const WaypointItemType* type() const;
    QVariantMap calcData();

    int count() const;
    int index(WaypointItem* item) const;
    WaypointItem* item(int index) const;
    const QList<WaypointItem*>& items() const;

public slots:
    void setType(const WaypointItemType* type);

    void setAndCheckParameter(const QString& paramId, const QVariant& value);
    void resetParameter(const QString& paramId);
    void resetParameters();
    void syncParameters();

    void setCalcData(const QVariantMap& calcData);

    void setItems(const QList<WaypointItem*>& items);
    void addItem(WaypointItem* item);
    void removeItem(WaypointItem* item);

signals:
    void itemAdded(int index, WaypointItem* item);
    void itemChanged(int index, WaypointItem* item);
    void itemRemoved(int index, WaypointItem* item);

signals:
    void typeChanged();

private:
    const WaypointItemType* m_type;
    QVariantMap m_calcData;
    QList<WaypointItem*> m_items;
};
} // namespace md::domain

#endif // WAYPOINT_ITEM_H
