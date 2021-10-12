#include "vehicles_service.h"

using namespace md::domain;

// TODO: db storage
VehiclesService::VehiclesService(QObject* parent) : IVehiclesService(parent)
{
}

Vehicle* VehiclesService::vehicle(const QVariant& id) const
{
    return m_vehicles.value(id, nullptr);
}

QList<Vehicle*> VehiclesService::vehicles() const
{
    return m_vehicles.values();
}

void VehiclesService::readAllVehicles()
{
}

void VehiclesService::removeVehicle(Vehicle* vehicle)
{
    if (!m_vehicles.contains(vehicle->id()))
        return;

    m_vehicles.remove(vehicle->id());
    emit vehicleRemoved(vehicle);
    vehicle->deleteLater();
}

void VehiclesService::restoreVehicle(Vehicle* vehicle)
{
}

void VehiclesService::saveVehicle(Vehicle* vehicle)
{
    if (!m_vehicles.contains(vehicle->id()))
    {
        vehicle->moveToThread(this->thread());
        vehicle->setParent(this);
        m_vehicles.insert(vehicle->id(), vehicle);
        emit vehicleAdded(vehicle);
    }
}
