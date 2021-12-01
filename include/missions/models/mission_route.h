#ifndef MISSION_ROUTE_H
#define MISSION_ROUTE_H

#include "mission_route_item.h"
#include "route.h"

namespace md::domain
{
class MissionRoute : public Entity
{
    Q_OBJECT

public:
    MissionRoute(Route* underlyingRoute, QObject* parent = nullptr);

    Route* underlyingRoute() const;

    int count() const;
    QList<MissionRouteItem*> items() const;
    MissionRouteItem* item(int index) const;

    int currentItem() const;

public slots:
    void addNewItem(MissionRouteItem* item);
    void clear();

    void setCurrentItem(int currentItem);

signals:
    void itemsChanged();
    void currentItemChanged(int index);

private:
    Route* const m_underlyingRoute;
    QList<MissionRouteItem*> m_items;
    int m_currentItem = -1;
};
} // namespace md::domain

#endif // MISSION_ROUTE_H
