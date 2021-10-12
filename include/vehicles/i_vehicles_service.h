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
    virtual QList<Vehicle*> vehicles() const = 0;

public slots:
    virtual void readAllVehicles() = 0;
    virtual void removeVehicle(Vehicle* vehicle) = 0;
    virtual void restoreVehicle(Vehicle* vehicle) = 0;
    virtual void saveVehicle(Vehicle* vehicle) = 0;

signals:
    void vehicleAdded(Vehicle* vehicle);
    void vehicleChanged(Vehicle* vehicle);
    void vehicleRemoved(Vehicle* vehicle);
};
} // namespace md::domain

#endif // I_VEHICLES_SERVICE_H
