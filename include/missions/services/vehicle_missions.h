#ifndef VEHICLE_MISSIONS_H
#define VEHICLE_MISSIONS_H

#include "i_vehicle_missions.h"
#include "i_vehicles_service.h"

namespace md::domain
{
class VehicleMissions : public IVehicleMissions
{
    Q_OBJECT

public:
    VehicleMissions(IMissionsService* missions, IVehiclesService* vehicles,
                    QObject* parent = nullptr);

    Mission* missionForVehicle(const QVariant& vehicleId) const override;

    void registerMissionTypeForVehicleType(const QString& vehicleTypeId,
                                           const QString& missionTypeId) override;
    void unregisterMissionTypeForVehicleType(const QString& vehicleTypeId) override;

public slots:
    void obtainVehicle(domain::Vehicle* vehicle) override;

private slots:
    void onMissionAdded(domain::Mission* mission);
    void onMissionRemoved(domain::Mission* mission);
    void onVehicleAdded(domain::Vehicle* vehicle);
    void onVehicleRemoved(domain::Vehicle* vehicle);

private:
    IMissionsService* const m_missions;

    QMap<QString, QString> m_typeMap;
    QMap<QVariant, Mission*> m_vehicleMissionMap;
};
} // namespace md::domain

#endif // VEHICLE_MISSIONS_H
