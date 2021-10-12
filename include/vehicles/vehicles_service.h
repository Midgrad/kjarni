#ifndef VEHICLES_SERVICE_H
#define VEHICLES_SERVICE_H

#include "i_vehicles_service.h"

#include <QMap>

namespace md::domain
{
class VehiclesService : public IVehiclesService
{
    Q_OBJECT

public:
    VehiclesService(QObject* parent = nullptr);

    Vehicle* vehicle(const QVariant& id) const override;
    QList<Vehicle*> vehicles() const override;

public slots:
    void readAllVehicles() override;
    void removeVehicle(Vehicle* vehicle) override;
    void restoreVehicle(Vehicle* vehicle) override;
    void saveVehicle(Vehicle* vehicle) override;

private:
    QMap<QVariant, Vehicle*> m_vehicles;
};
} // namespace md::domain

#endif // VEHICLES_SERVICE_H
