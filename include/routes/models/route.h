#ifndef ROUTE_H
#define ROUTE_H

#include "route_item.h"
#include "route_type.h"

namespace md::domain
{
class Route : public Named
{
    Q_OBJECT

public:
    Route(const RouteType* type, const QString& name = QString(),
          const QVariant& id = utils::generateId(), QObject* parent = nullptr);
    Route(const RouteType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    const RouteType* type() const;

    int count() const;
    int index(RouteItem* item) const;
    const QList<RouteItem*>& items() const;
    RouteItem* item(int index) const;

public slots:
    void setItems(const QList<RouteItem*>& items);
    void addItem(RouteItem* item);
    void removeItem(RouteItem* item);
    void clear();

signals:
    void itemAdded(int index, RouteItem* item);
    void itemChanged(int index, RouteItem* item);
    void itemRemoved(int index, RouteItem* item);

private:
    const RouteType* const m_type;
    QList<RouteItem*> m_items;
};
} // namespace md::domain

#endif // ROUTE_H
