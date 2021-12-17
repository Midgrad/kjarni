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
            const QVariant& id = utils::generateId(), const QVariant& homeId = utils::generateId(),
            QObject* parent = nullptr);
    Mission(const MissionType* type, const QVariantMap& map, QObject* parent = nullptr);

    utils::ConstProperty<MissionType const*> type;
    utils::ConstProperty<QVariant> vehicleId;
    utils::ConstProperty<RouteItem*> home;

    QVariantMap toVariantMap() const override;

    Route* route() const;
    RouteItem* item(int index);
    int count() const;
    int currentItem() const;

public slots:
    void assignRoute(Route* route);
    void setReached(int index);
    void setCurrentItem(int index);
    void clear();

signals:
    void routeChanged(Route* route);
    void currentItemChanged(int index);
    void itemReached(int index);

    void switchCurrentItem(int item); // Goto item

private:
    Route* m_route = nullptr;
    RouteItem* m_currentItem = nullptr;
};
} // namespace md::domain

#endif // MISSION_H
