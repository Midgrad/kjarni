#ifndef ROUTE_ITEM_H
#define ROUTE_ITEM_H

#include "entity.h"
#include "route_item_type.h"

namespace md::domain
{
class RouteItem : public Parametrised
{
    Q_OBJECT

public:
    RouteItem(const RouteItemType* type, const QVariant& id = utils::generateId(),
              QObject* parent = nullptr);
    RouteItem(const RouteItemType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    const RouteItemType* type() const;
    QVariantMap calcData();
    bool isCurrent() const;
    bool isReached() const;

    int count() const;
    int index(RouteItem* item) const;
    RouteItem* item(int index) const;
    const QList<RouteItem*>& items() const;

public slots:
    void setType(const RouteItemType* type);

    void setAndCheckParameter(const QString& paramId, const QVariant& value);
    void resetParameter(const QString& paramId);
    void resetParameters();
    void syncParameters();

    void setCalcData(const QVariantMap& calcData);
    void setCurrent(bool current);
    void setReached(bool reached);

    void setItems(const QList<RouteItem*>& items);
    void addItem(RouteItem* item);
    void removeItem(RouteItem* item);

signals:
    void itemAdded(int index, RouteItem* item);
    void itemChanged(int index, RouteItem* item);
    void itemRemoved(int index, RouteItem* item);

signals:
    void typeChanged();

private:
    const RouteItemType* m_type;
    QVariantMap m_calcData;
    bool m_current = false;
    bool m_reached = false;

    QList<RouteItem*> m_items;
};
} // namespace md::domain

#endif // ROUTE_ITEM_H
