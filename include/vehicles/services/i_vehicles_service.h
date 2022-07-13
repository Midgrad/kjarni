#ifndef I_VEHICLES_SERVICE_H
#define I_VEHICLES_SERVICE_H

#include "i_service.h"
#include "vehicle.h"

namespace md::domain
{
class IVehiclesService
    : public QObject
    , public IService
{
    Q_OBJECT

public:
    IVehiclesService(QObject* parent) : QObject(parent), IService()
    {
    }

    virtual Vehicle* vehicle(const QVariant& id) const = 0;
    virtual QVariantList vehicleIds() const = 0;
    virtual QList<Vehicle*> vehicles() const = 0;
    virtual QList<const VehicleType*> vehicleTypes() const = 0;
    virtual const VehicleType* vehicleType(const QString& typeId) const = 0;

public slots:
    virtual void readAll() = 0;
    virtual void removeVehicle(md::domain::Vehicle* vehicle) = 0;
    virtual void restoreVehicle(md::domain::Vehicle* vehicle) = 0;
    virtual void saveVehicle(md::domain::Vehicle* vehicle) = 0;
    virtual void addVehicleType(const md::domain::VehicleType* type) = 0;
    virtual void removeVehicleType(const md::domain::VehicleType* type) = 0;

signals:
    void vehicleAdded(md::domain::Vehicle* vehicle);
    void vehicleChanged(md::domain::Vehicle* vehicle);
    void vehicleRemoved(md::domain::Vehicle* vehicle);
    void vehicleTypesChanged();
};
} // namespace md::domain

#endif // I_VEHICLES_SERVICE_H
