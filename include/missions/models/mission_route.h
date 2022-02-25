#ifndef MISSION_ROUTE_H
#define MISSION_ROUTE_H

#include "mission_route_item.h"
#include "visible_mixin.hpp"

namespace md::domain
{
class MissionRoute : public NamedMixin<VisibleMixin<Entity>>
{
    Q_OBJECT

public:
    explicit MissionRoute(const QString& name, const QVariant& id = utils::generateId(),
                          QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    int count() const;
    int index(MissionRouteItem* item) const;
    const QList<MissionRouteItem*>& items() const;
    MissionRouteItem* item(int index) const;

public slots:
    void replaceItem(int index, md::domain::MissionRouteItem* item);
    void addItem(md::domain::MissionRouteItem* item);
    void removeItem(md::domain::MissionRouteItem* item);
    void clear();

signals:
    void itemAdded(int index, md::domain::MissionRouteItem* item);
    void itemChanged(int index, md::domain::MissionRouteItem* item);
    void itemRemoved(int index, md::domain::MissionRouteItem* item);

    void goTo(int index);

private:
    QList<MissionRouteItem*> m_items;
};
} // namespace md::domain

#endif // MISSION_ROUTE_H
