#ifndef MISSION_H
#define MISSION_H

#include "mission_type.h"
#include "route.h"

namespace md::domain
{
class Mission : public Named
{
    Q_OBJECT

public:
    Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
            const QVariant& id = utils::generateId(), QObject* parent = nullptr);
    Mission(const MissionType* type, const QVariantMap& map, QObject* parent = nullptr);

    utils::ConstProperty<MissionType const*> type;
    utils::ConstProperty<QVariant> vehicleId;
    utils::ConstProperty<RouteItem*> home;
    utils::Property<Route*> route;

    QVariantMap toVariantMap() const override;

    RouteItem* item(int index);
    int count() const;
    int currentItem() const;

public slots:
    void setReached(int index);
    void setCurrentItem(int index);
    void clear();

signals:
    void routeChanged(Route* route);
    void currentItemChanged(int index);
    void itemReached(int index);

    void switchCurrentItem(int item); // Goto item

private:
    RouteItem* m_currentItem = nullptr;
};
} // namespace md::domain

#endif // MISSION_H
