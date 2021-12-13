#ifndef ROUTE_ITEM_H
#define ROUTE_ITEM_H

#include "entity.h"
#include "geodetic.h"
#include "route_item_type.h"

namespace md::domain
{
class RouteItem : public Parametrised
{
    Q_OBJECT

public:
    RouteItem(const RouteItemType* type, const QVariant& id = utils::generateId(),
              const QString& name = QString(), QVariantMap params = {},
              const Geodetic& position = Geodetic(), const QVariantMap& calcData = {},
              bool current = false, bool reached = false, QObject* parent = nullptr);
    RouteItem(const RouteItemType* type, const QVariantMap& map, QObject* parent = nullptr);

    utils::Property<Geodetic> position;
    utils::Property<QVariantMap> calcData;
    utils::Property<bool> current;
    utils::Property<bool> reached;

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    const RouteItemType* type() const;

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

    QList<RouteItem*> m_items;
};
} // namespace md::domain

#endif // ROUTE_ITEM_H
