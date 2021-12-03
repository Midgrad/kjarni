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
            QObject* parent = nullptr);
    Mission(const MissionType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;

    const MissionType* type() const;
    QVariant vehicleId() const;

    RouteItem* home() const;
    Route* route() const;

    int currentItem() const;

public slots:
    void assignRoute(Route* route);
    void setCurrentItem(int currentItem);
    void clear();

signals:
    void routeChanged();
    void currentItemChanged();

    void switchCurrentItem(int item); // Goto item

private:
    const MissionType* const m_type;
    const QVariant m_vehicleId;

    int m_currentItem = -1;

    RouteItem* const m_home;
    Route* m_route;
};
} // namespace md::domain

#endif // MISSION_H
