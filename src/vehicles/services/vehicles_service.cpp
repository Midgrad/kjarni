#include "vehicles_service.h"

#include <QDebug>
#include <QMutexLocker>

using namespace md::domain;

VehiclesService::VehiclesService(IVehiclesRepository* vehiclesRepo, QObject* parent) :
    IVehiclesService(parent),
    m_vehiclesRepo(vehiclesRepo),
    m_mutex(QMutex::Recursive)
{
    qRegisterMetaType<Vehicle*>("md::domain::Vehicle*");
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

QList<const VehicleType*> VehiclesService::vehicleTypes() const
{
    return m_vehicleTypes.values();
}

const VehicleType* VehiclesService::vehicleType(const QString& typeId) const
{
    return m_vehicleTypes.value(typeId, nullptr);
}

void VehiclesService::readAll()
{
    QMutexLocker locker(&m_mutex);

    for (const QVariant& vehicleId : m_vehiclesRepo->selectVehicleIds())
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

    m_vehiclesRepo->remove(vehicle);
    m_vehicles.remove(vehicle->id);

    emit vehicleRemoved(vehicle);
    vehicle->deleteLater();
}

void VehiclesService::restoreVehicle(Vehicle* vehicle)
{
    QMutexLocker locker(&m_mutex);

    m_vehiclesRepo->read(vehicle);
    emit vehicleChanged(vehicle);
}

void VehiclesService::saveVehicle(Vehicle* vehicle)
{
    QMutexLocker locker(&m_mutex);

    if (m_vehicles.contains(vehicle->id))
    {
        m_vehiclesRepo->update(vehicle);
        emit vehicleChanged(vehicle);
    }
    else
    {
        m_vehiclesRepo->insert(vehicle);
        m_vehicles.insert(vehicle->id, vehicle);
        vehicle->moveToThread(this->thread());
        vehicle->setParent(this);
        emit vehicleAdded(vehicle);
    }
}

void VehiclesService::addVehicleType(const VehicleType* type)
{
    if (m_vehicleTypes.contains(type->id))
        return;

    m_vehicleTypes.insert(type->id, type);
    emit vehicleTypesChanged();
}

void VehiclesService::removeVehicleType(const VehicleType* type)
{
    if (!m_vehicleTypes.contains(type->id))
        return;

    m_vehicleTypes.remove(type->id);
    emit vehicleTypesChanged();
}

Vehicle* VehiclesService::readVehicle(const QVariant& id)
{
    QVariantMap select = m_vehiclesRepo->select(id);
    QString typeId = select.value(props::type).toString();
    const VehicleType* const type = m_vehicleTypes.value(typeId);
    if (!type)
    {
        qWarning() << "Unknown vehicle type" << typeId;
        return nullptr;
    }

    Vehicle* vehicle = new Vehicle(type, select, this);

    m_vehicles.insert(id, vehicle);
    emit vehicleAdded(vehicle);

    return vehicle;
}
