#include "vehicles_repository_sql.h"

#include <QDebug>

namespace
{
constexpr char vehicles[] = "vehicles";
} // namespace

using namespace md::data_source;

VehiclesRepositorySql::VehiclesRepositorySql(QSqlDatabase* database) :
    IVehiclesRepository(),
    m_vehiclesTable(database, ::vehicles, { domain::props::params })
{
}

QVariantList VehiclesRepositorySql::selectVehicleIds()
{
    return m_vehiclesTable.selectIds();
}

QVariantMap VehiclesRepositorySql::select(const QVariant& vehicleId)
{
    return m_vehiclesTable.selectById(vehicleId);
}

void VehiclesRepositorySql::insert(domain::Vehicle* vehicle)
{
    m_vehiclesTable.insertEntity(vehicle);
}

void VehiclesRepositorySql::read(domain::Vehicle* vehicle)
{
    m_vehiclesTable.readEntity(vehicle);
}

void VehiclesRepositorySql::update(domain::Vehicle* vehicle)
{
    m_vehiclesTable.updateEntity(vehicle);
}

void VehiclesRepositorySql::remove(domain::Vehicle* vehicle)
{
    m_vehiclesTable.removeEntity(vehicle);
}
