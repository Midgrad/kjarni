#ifndef VEHICLES_REPOSITORY_H
#define VEHICLES_REPOSITORY_H

#include "entity_sql_table.h"
#include "i_vehicles_repository.h"

#include <QMap>
#include <QMutex>

namespace md::domain
{
class VehiclesRepositorySql : public IVehiclesRepository
{
    Q_OBJECT

public:
    VehiclesRepositorySql(QSqlDatabase* database, QObject* parent = nullptr);

    Vehicle* vehicle(const QVariant& id) const override;
    QVariantList vehicleIds() const override;
    QList<Vehicle*> vehicles() const override;

public slots:
    void readAll() override;
    void removeVehicle(Vehicle* vehicle) override;
    void restoreVehicle(Vehicle* vehicle) override;
    void saveVehicle(Vehicle* vehicle) override;

private:
    Vehicle* readVehicle(const QVariant& id);

    data_source::EntitySqlTable m_vehiclesTable;
    QMap<QVariant, Vehicle*> m_vehicles;

    mutable QMutex m_mutex;
};
} // namespace md::domain

#endif // VEHICLES_REPOSITORY_H
