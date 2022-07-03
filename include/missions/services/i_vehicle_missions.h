#ifndef I_VEHICLE_MISSIONS_H
#define I_VEHICLE_MISSIONS_H

#include "i_missions_service.h"
#include "vehicle.h"

namespace md::domain
{
class IVehicleMissions
    : public QObject
    , public IService
{
    Q_OBJECT

public:
    IVehicleMissions(QObject* parent) : QObject(parent), IService()
    {
    }

    virtual Mission* missionForVehicle(const QVariant& vehicleId) const = 0;

    virtual void registerMissionTypeForVehicleType(const QString& vehicleTypeId,
                                                   const QString& missionTypeId) = 0;
    virtual void unregisterMissionTypeForVehicleType(const QString& vehicleTypeId) = 0;

public slots:
    virtual void obtainVehicle(domain::Vehicle* vehicle) = 0;

signals:
    void missionAdded(domain::Mission* mission);
    void missionRemoved(domain::Mission* mission);
};
} // namespace md::domain

#endif // I_VEHICLE_MISSIONS_H
