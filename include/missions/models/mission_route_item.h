#ifndef MISSION_ROUTE_ITEM_H
#define MISSION_ROUTE_ITEM_H

#include "route_item.h"

namespace md::domain
{
class MissionRouteItem : public Entity
{
    Q_OBJECT

public:
    MissionRouteItem(RouteItem* underlyingItem, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    RouteItem* underlyingItem() const;

    bool current() const;
    bool reached() const;
    bool confirmed() const;

public slots:
    void setCurrent(bool current);
    void setReached(bool reached);
    void setConfirmed(bool confirmed);

private:
    RouteItem* const m_underlyingItem;

    bool m_current;
    bool m_reached;
    bool m_confirmed;
};
} // namespace md::domain

#endif // MISSION_ROUTE_ITEM_H
