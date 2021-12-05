#ifndef VEHICLES_REPOSITORY_SQL_H
#define VEHICLES_REPOSITORY_SQL_H

#include "entity_sql_table.h"
#include "i_vehicles_repository.h"

namespace md::data_source
{
class VehiclesRepositorySql : public domain::IVehiclesRepository
{
public:
    VehiclesRepositorySql(QSqlDatabase* database);

    QVariantList selectVehicleIds() override;
    QVariantMap select(const QVariant& vehicleId) override;

    void insert(domain::Vehicle* vehicle) override;
    void read(domain::Vehicle* vehicle) override;
    void update(domain::Vehicle* vehicle) override;
    void remove(domain::Vehicle* vehicle) override;

private:
    EntitySqlTable m_vehiclesTable;
};
} // namespace md::data_source

#endif // VEHICLES_REPOSITORY_SQL_H
