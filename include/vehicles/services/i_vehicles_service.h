#ifndef I_VEHICLES_SERVICE_H
#define I_VEHICLES_SERVICE_H

#include "i_service.h"
#include "vehicle.h"

namespace md::domain
{
class IVehiclesService // TODO: rename to Context, Unit of Work or Registry
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
    virtual QStringList vehicleTypes() const = 0;

public slots:
    virtual void readAll() = 0;
    virtual void removeVehicle(Vehicle* vehicle) = 0;
    virtual void restoreVehicle(Vehicle* vehicle) = 0;
    virtual void saveVehicle(Vehicle* vehicle) = 0;
    virtual void addVehicleType(const QString& type) = 0;
    virtual void removeVehicleType(const QString& type) = 0;

signals:
    void vehicleAdded(Vehicle* vehicle);
    void vehicleChanged(Vehicle* vehicle);
    void vehicleRemoved(Vehicle* vehicle);
    void vehicleTypesChanged();
};
} // namespace md::domain

#endif // I_VEHICLES_SERVICE_H
