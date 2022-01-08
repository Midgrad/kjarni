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
    QStringList vehicleTypes() const override;

public slots:
    void readAll() override;
    void removeVehicle(Vehicle* vehicle) override;
    void restoreVehicle(Vehicle* vehicle) override;
    void saveVehicle(Vehicle* vehicle) override;
    void addVehicleType(const QString& type) override;
    void removeVehicleType(const QString& type) override;

private:
    Vehicle* readVehicle(const QVariant& id);

    QStringList m_vehicleTypes;
    IVehiclesRepository* const m_vehiclesRepo;
    QMap<QVariant, Vehicle*> m_vehicles;

    mutable QMutex m_mutex;
};
} // namespace md::domain

#endif // VEHICLES_SERVICE_H
