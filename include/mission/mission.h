#ifndef MISSION_H
#define MISSION_H

#include "mission_status.h"
#include "mission_traits.h"
#include "mission_type.h"
#include "route.h"

namespace md::domain
{
class Mission : public Entity
{
    Q_OBJECT

public:
    Mission(const MissionType* type, const QString& name, const QUuid& vehicleId,
            QObject* parent = nullptr);
    Mission(const QVariantMap& map, const MissionType* type, QObject* parent = nullptr);

    QVariantMap toVariantMap(bool recursive) const override;
    void fromVariantMap(const QVariantMap& map) override;

    const MissionType* type() const;
    QUuid vehicleId() const;
    Route* route() const;
    const MissionStatus& status() const;
    int currentWaypoint() const;

public slots:
    void updateStatus(MissionStatus::Type type = MissionStatus::NotActual, int progress = -1,
                      int total = -1);
    void updateStatusProgress(int progress);
    void setCurrentWaypoint(int currentWaypoint);

signals:
    void vehicleChanged(QUuid vehicle);
    void routeChanged(Route* route);
    void statusChanged(MissionStatus status);
    void currentWaypointChanged(int currentWaypoint);

    void upload();   // To the vehicle
    void download(); // From the vehicle
    void cancel();   // Downloading or uploading
    void clear();    // Clear mission onborad and in storage
    void switchWaypoint(int waypoint);

private:
    const MissionType* m_type;
    const QUuid m_vehicleId;
    Route* const m_route;
    MissionStatus m_status;
    int m_currentWaypoint = -1;
};
} // namespace md::domain

#endif // MISSION_H