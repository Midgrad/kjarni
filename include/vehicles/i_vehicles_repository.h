#ifndef I_VEHICLES_REPOSITORY_H
#define I_VEHICLES_REPOSITORY_H

#include "i_service.h"
#include "vehicle.h"

namespace md::domain
{
class IVehiclesRepository
    : public QObject
    , public IService
{
    Q_OBJECT

public:
    IVehiclesRepository(QObject* parent) : QObject(parent), IService()
    {
    }

    virtual Vehicle* vehicle(const QVariant& id) const = 0;
    virtual QVariantList vehicleIds() const = 0;
    virtual QList<Vehicle*> vehicles() const = 0;

public slots:
    virtual void readAll() = 0;
    virtual void removeVehicle(Vehicle* vehicle) = 0;
    virtual void restoreVehicle(Vehicle* vehicle) = 0;
    virtual void saveVehicle(Vehicle* vehicle) = 0;

signals:
    void vehicleAdded(Vehicle* vehicle);
    void vehicleChanged(Vehicle* vehicle);
    void vehicleRemoved(Vehicle* vehicle);
};
} // namespace md::domain

#endif // I_VEHICLES_REPOSITORY_H
