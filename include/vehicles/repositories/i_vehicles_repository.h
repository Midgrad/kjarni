#ifndef I_VEHICLES_REPOSITORY_H
#define I_VEHICLES_REPOSITORY_H

#include "vehicle.h"

namespace md::domain
{
class IVehiclesRepository
{
public:
    IVehiclesRepository() = default;
    virtual ~IVehiclesRepository() = default;

    virtual QVariantList selectVehicleIds() = 0;
    virtual QVariantMap select(const QVariant& vehicleId) = 0;

    virtual void insert(Vehicle* vehicle) = 0;
    virtual void read(Vehicle* vehicle) = 0;
    virtual void update(Vehicle* vehicle) = 0;
    virtual void remove(Vehicle* vehicle) = 0;
};
} // namespace md::domain

#endif // I_VEHICLES_REPOSITORY_H
