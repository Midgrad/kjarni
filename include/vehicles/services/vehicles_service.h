#ifndef VEHICLES_SERVICE_H
#define VEHICLES_SERVICE_H

#include "i_vehicles_repository.h"
#include "i_vehicles_service.h"

#include <QMutex>

namespace md::domain
{
class VehiclesService : public IVehiclesService
{
    Q_OBJECT

public:
    VehiclesService(IVehiclesRepository* vehiclesRepo, QObject* parent = nullptr);

    Vehicle* vehicle(const QVariant& id) const override;
    QVariantList vehicleIds() const override;
    QList<Vehicle*> vehicles() const override;
    QList<const VehicleType*> vehicleTypes() const override;
    const VehicleType* vehicleType(const QString& typeId) const override;

public slots:
    void readAll() override;
    void removeVehicle(Vehicle* vehicle) override;
    void restoreVehicle(Vehicle* vehicle) override;
    void saveVehicle(Vehicle* vehicle) override;
    void addVehicleType(const VehicleType* type) override;
    void removeVehicleType(const VehicleType* type) override;

private:
    Vehicle* readVehicle(const QVariant& id);

    IVehiclesRepository* const m_vehiclesRepo;
    QMap<QString, const VehicleType*> m_vehicleTypes;
    QMap<QVariant, Vehicle*> m_vehicles;

    mutable QMutex m_mutex;
};
} // namespace md::domain

#endif // VEHICLES_SERVICE_H
