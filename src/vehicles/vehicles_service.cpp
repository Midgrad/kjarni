#include "vehicles_service.h"

#include <QDebug>
#include <QMutexLocker>

namespace
{
constexpr char vehicles[] = "vehicles";
} // namespace

using namespace md::domain;

VehiclesService::VehiclesService(IRepositoryFactory* repoFactory, QObject* parent) :
    IVehiclesService(parent),
    m_vehiclesRepo(repoFactory->create(::vehicles))
{
}

Vehicle* VehiclesService::vehicle(const QVariant& id) const
{
    QMutexLocker locker(&m_mutex);
    return m_vehicles.value(id, nullptr);
}

QVariantList VehiclesService::vehicleIds() const
{
    QMutexLocker locker(&m_mutex);
    return m_vehicles.keys();
}

QList<Vehicle*> VehiclesService::vehicles() const
{
    QMutexLocker locker(&m_mutex);
    return m_vehicles.values();
}

void VehiclesService::readAll()
{
    QMutexLocker locker(&m_mutex);

    for (const QVariant& vehicleId : m_vehiclesRepo->selectIds())
    {
        if (!m_vehicles.contains(vehicleId))
        {
            this->readVehicle(vehicleId);
        }
    }
}

void VehiclesService::removeVehicle(Vehicle* vehicle)
{
    QMutexLocker locker(&m_mutex);

    if (!vehicle->id().isNull())
        m_vehiclesRepo->remove(vehicle);

    m_vehicles.remove(vehicle->id());

    emit vehicleRemoved(vehicle);
    vehicle->deleteLater();
}

void VehiclesService::restoreVehicle(Vehicle* vehicle)
{
    QMutexLocker locker(&m_mutex);

    if (vehicle->id().isNull())
        return;

    m_vehiclesRepo->read(vehicle);
    emit vehicleChanged(vehicle);
}

void VehiclesService::saveVehicle(Vehicle* vehicle)
{
    QMutexLocker locker(&m_mutex);

    if (vehicle->id().isNull())
    {
        qWarning() << "Can't save vehicle with no id" << vehicle;
        return;
    }

    if (m_vehicles.contains(vehicle->id()))
    {
        m_vehiclesRepo->update(vehicle);
        emit vehicleChanged(vehicle);
    }
    else
    {
        m_vehiclesRepo->insert(vehicle);
        m_vehicles.insert(vehicle->id(), vehicle);
        vehicle->moveToThread(this->thread());
        vehicle->setParent(this);
        emit vehicleAdded(vehicle);
    }
}

Vehicle* VehiclesService::readVehicle(const QVariant& id)
{
    QVariantMap map = m_vehiclesRepo->select(id);
    QString typeName = map.value(params::type).toString();

    Vehicle* vehicle = new Vehicle(map, this);
    m_vehicles.insert(id, vehicle);
    emit vehicleAdded(vehicle);

    return vehicle;
}
