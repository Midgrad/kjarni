#ifndef MISSION_H
#define MISSION_H

#include "mission_status.h"
#include "mission_type.h"
#include "route.h"

namespace md::domain
{
class Mission : public Entity
{
    Q_OBJECT

public:
    Mission(const MissionType& type, const QVariant& id, const QString& name,
            QObject* parent = nullptr);
    Mission(const QJsonObject& json, const MissionType& type, QObject* parent = nullptr);

    QJsonObject toJson(bool recursive) const override;
    void fromJson(const QJsonObject& json) override;

    const MissionType& type() const;
    QString vehicle() const;
    Route* route() const;
    const MissionStatus& status() const;
    int currentWaypoint() const;

public slots:
    void setVehicle(const QString& vehicle); // TODO: const vehicle
    void updateStatus(MissionStatus::Type type = MissionStatus::NotActual, int progress = -1,
                      int total = -1);
    void updateStatusProgress(int progress);
    void setCurrentWaypoint(int currentWaypoint);

signals:
    void vehicleChanged(QString vehicle);
    void routeChanged(Route* route);
    void statusChanged(MissionStatus status);
    void currentWaypointChanged(int currentWaypoint);

    void upload();   // To the vehicle
    void download(); // From the vehicle
    void cancel();   // Downloading or uploading
    void clear();    // Clear mission onborad and in storage
    void switchWaypoint(int waypoint);

private:
    const MissionType m_type;
    QString m_vehicle;
    Route* const m_route;
    MissionStatus m_status;
    int m_currentWaypoint = -1;
};
} // namespace md::domain

#endif // MISSION_H
