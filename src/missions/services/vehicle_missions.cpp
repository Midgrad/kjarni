#include "vehicle_missions.h"

#include <QDebug>

using namespace md::domain;

VehicleMissions::VehicleMissions(IMissionsService* missions, IVehiclesService* vehicles,
                                 QObject* parent) :
    IVehicleMissions(parent),
    m_missions(missions)
{
    Q_ASSERT(missions);
    Q_ASSERT(vehicles);

    connect(missions, &IMissionsService::missionAdded, this, &VehicleMissions::onMissionAdded);
    connect(missions, &IMissionsService::missionRemoved, this, &VehicleMissions::onMissionRemoved);

    connect(vehicles, &IVehiclesService::vehicleAdded, this, &VehicleMissions::onVehicleAdded);
    connect(vehicles, &IVehiclesService::vehicleRemoved, this, &VehicleMissions::onVehicleRemoved);
}

Mission* VehicleMissions::missionForVehicle(const QVariant& vehicleId) const
{
    return m_vehicleMissionMap.value(vehicleId, nullptr);
}

void VehicleMissions::registerMissionTypeForVehicleType(const QString& vehicleTypeId,
                                                        const QString& missionTypeId)
{
    m_typeMap.insert(vehicleTypeId, missionTypeId);
}

void VehicleMissions::unregisterMissionTypeForVehicleType(const QString& vehicleTypeId)
{
    m_typeMap.remove(vehicleTypeId);
}

void VehicleMissions::obtainVehicle(Vehicle* vehicle)
{
    Mission* mission = this->missionForVehicle(vehicle->id);
    if (!mission)
        return;

    // Automaticaly download mission TODO: to settings
    m_missions->startOperation(mission, MissionOperation::Download);
}

void VehicleMissions::onMissionAdded(Mission* mission)
{
    m_vehicleMissionMap.insert(mission->vehicleId, mission);
    emit missionAdded(mission);
}

void VehicleMissions::onMissionRemoved(Mission* mission)
{
    m_vehicleMissionMap.remove(mission->vehicleId);
    emit missionRemoved(mission);
}

void VehicleMissions::onVehicleAdded(Vehicle* vehicle)
{
    if (this->missionForVehicle(vehicle->id))
        return;

    QString missionTypeId = m_typeMap.value(vehicle->type()->id);
    const MissionType* missionType = m_missions->missionType(missionTypeId);
    if (!missionType)
        return;

    Mission* mission = new Mission(missionType, tr("%1 mission").arg(vehicle->name), vehicle->id);
    m_missions->saveMission(mission);
}

void VehicleMissions::onVehicleRemoved(Vehicle* vehicle)
{
    Mission* mission = this->missionForVehicle(vehicle->id);
    if (!mission)
        return;

    m_missions->removeMission(mission);
}
