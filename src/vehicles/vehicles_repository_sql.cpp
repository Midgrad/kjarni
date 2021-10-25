#include "vehicles_repository_sql.h"

#include <QDebug>
#include <QMutexLocker>

namespace
{
constexpr char vehicles[] = "vehicles";
} // namespace

using namespace md::domain;

VehiclesRepositorySql::VehiclesRepositorySql(QSqlDatabase* database, QObject* parent) :
    IVehiclesRepository(parent),
    m_vehiclesTable(database, ::vehicles)
{
}

Vehicle* VehiclesRepositorySql::vehicle(const QVariant& id) const
{
    QMutexLocker locker(&m_mutex);
    return m_vehicles.value(id, nullptr);
}

QVariantList VehiclesRepositorySql::vehicleIds() const
{
    QMutexLocker locker(&m_mutex);
    return m_vehicles.keys();
}

QList<Vehicle*> VehiclesRepositorySql::vehicles() const
{
    QMutexLocker locker(&m_mutex);
    return m_vehicles.values();
}

void VehiclesRepositorySql::readAll()
{
    QMutexLocker locker(&m_mutex);

    for (const QVariant& vehicleId : m_vehiclesTable.selectIds())
    {
        if (!m_vehicles.contains(vehicleId))
        {
            this->readVehicle(vehicleId);
        }
    }
}

void VehiclesRepositorySql::removeVehicle(Vehicle* vehicle)
{
    QMutexLocker locker(&m_mutex);

    if (!vehicle->id().isNull())
        m_vehiclesTable.removeEntity(vehicle);

    m_vehicles.remove(vehicle->id());

    emit vehicleRemoved(vehicle);
    vehicle->deleteLater();
}

void VehiclesRepositorySql::restoreVehicle(Vehicle* vehicle)
{
    QMutexLocker locker(&m_mutex);

    if (vehicle->id().isNull())
        return;

    m_vehiclesTable.readEntity(vehicle);
    emit vehicleChanged(vehicle);
}

void VehiclesRepositorySql::saveVehicle(Vehicle* vehicle)
{
    QMutexLocker locker(&m_mutex);

    if (vehicle->id().isNull())
    {
        qWarning() << "Can't save vehicle with no id" << vehicle;
        return;
    }

    if (m_vehicles.contains(vehicle->id()))
    {
        m_vehiclesTable.updateEntity(vehicle);
        emit vehicleChanged(vehicle);
    }
    else
    {
        m_vehiclesTable.insertEntity(vehicle);
        m_vehicles.insert(vehicle->id(), vehicle);
        vehicle->moveToThread(this->thread());
        vehicle->setParent(this);
        emit vehicleAdded(vehicle);
    }
}

Vehicle* VehiclesRepositorySql::readVehicle(const QVariant& id)
{
    QVariantMap map = m_vehiclesTable.selectById(id);
    QString typeName = map.value(params::type).toString();

    Vehicle* vehicle = new Vehicle(map, this);
    m_vehicles.insert(id, vehicle);
    emit vehicleAdded(vehicle);

    return vehicle;
}
