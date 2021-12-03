#ifndef MISSION_ROUTE_H
#define MISSION_ROUTE_H

#include "mission_route_item.h"
#include "mission_type.h"
#include "route.h"

namespace md::domain
{
class MissionRoute : public Entity // TODO: IRoute
{
    Q_OBJECT

public:
    MissionRoute(const MissionType* type, const QVariant& id, QObject* parent = nullptr);

    int count() const;
    MissionRouteItem* homeItem() const;
    QList<MissionRouteItem*> items() const;
    MissionRouteItem* item(int index) const;

    int currentItem() const;

    void copyTo(Route* route);
    void copyFrom(Route* route);

public slots:
    void addItem(MissionRouteItem* item);
    void removeItem(MissionRouteItem* item);
    void clear();

    void setCurrentItem(int currentItem);

signals:
    void itemsChanged();
    void currentItemChanged(int index);

    void switchCurrentItem(int item); // Goto item

private:
    const MissionType* const m_type;
    MissionRouteItem* const m_home;
    QList<MissionRouteItem*> m_items;
    int m_currentItem = -1;
};
} // namespace md::domain

#endif // MISSION_ROUTE_H
