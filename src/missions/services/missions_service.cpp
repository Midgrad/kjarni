#include "missions_service.h"

#include <QDebug>

#include "mission_traits.h"
#include "utils.h"

using namespace md::domain;

MissionsService::MissionsService(IRoutesService* routes, IMissionsRepository* missionsRepo,
                                 IHomeItemsRepository* itemsRepo, QObject* parent) :
    IMissionsService(parent),
    m_routes(routes),
    m_missionsRepo(missionsRepo),
    m_homeItemsRepo(itemsRepo),
    m_mutex(QMutex::Recursive)
{
}

Mission* MissionsService::mission(const QVariant& id) const
{
    QMutexLocker locker(&m_mutex);
    return m_missions.value(id, nullptr);
}

Mission* MissionsService::missionForVehicle(const QVariant& vehicleId) const
{
    QMutexLocker locker(&m_mutex);
    return this->mission(m_missionsRepo->selectMissionIdForVehicle(vehicleId));
}

QVariantList MissionsService::missionIds() const
{
    QMutexLocker locker(&m_mutex);
    return m_missions.keys();
}

QList<Mission*> MissionsService::missions() const
{
    QMutexLocker locker(&m_mutex);
    return m_missions.values();
}

MissionOperation* MissionsService::operationForMission(Mission* mission) const
{
    return m_operations.value(mission, nullptr);
}

const MissionType* MissionsService::missionType(const QString& id) const
{
    return m_missionTypes.value(id, nullptr);
}

QList<const MissionType*> MissionsService::missionTypes() const
{
    QMutexLocker locker(&m_mutex);
    return m_missionTypes.values();
}

void MissionsService::startOperation(Mission* mission, MissionOperation::Type type)
{
    QMutexLocker locker(&m_mutex);

    MissionOperation* operation = this->operationForMission(mission);
    if (operation)
        this->endOperation(operation, MissionOperation::Canceled);

    operation = new MissionOperation(type, mission);
    operation->moveToThread(this->thread());
    operation->setParent(this);

    emit operationStarted(operation);
}

void MissionsService::endOperation(MissionOperation* operation, MissionOperation::State state)
{
    QMutexLocker locker(&m_mutex);

    operation->state.set(state);
    emit operationEnded(operation);
    operation->deleteLater();
}

void MissionsService::registerMissionType(const MissionType* type)
{
    QMutexLocker locker(&m_mutex);

    if (m_missionTypes.contains(type->id))
        return;

    m_missionTypes.insert(type->id, type);
    emit missionTypesChanged();
}

void MissionsService::unregisterMissionType(const MissionType* type)
{
    QMutexLocker locker(&m_mutex);

    if (!m_missionTypes.contains(type->id))
        return;

    m_missionTypes.remove(type->id);
    emit missionTypesChanged();
}

void MissionsService::readAll()
{
    QMutexLocker locker(&m_mutex);

    for (const QVariant& missionId : m_missionsRepo->selectMissionIds())
    {
        if (!m_missions.contains(missionId))
        {
            this->readMission(missionId);
        }
    }
}

void MissionsService::removeMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);

    // Stop if we have mission operation
    MissionOperation* operation = this->operationForMission(mission);
    if (operation)
        this->endOperation(operation, MissionOperation::Canceled);

    // Remove home point
    m_homeItemsRepo->remove(mission->home);

    // Remove mission
    m_missionsRepo->remove(mission);
    m_missions.remove(mission->id);

    emit missionRemoved(mission);
    mission->deleteLater();
}

void MissionsService::restoreMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);

    // Restore mission
    m_missionsRepo->read(mission);

    // Restore home point
    m_homeItemsRepo->read(mission->home);

    emit missionChanged(mission);
}

void MissionsService::saveMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);

    // Threads nd parents
    mission->moveToThread(this->thread());
    mission->setParent(this);

    // TODO: remove extra cals to routes
    // Save mission route
    if (mission->route())
        m_routes->saveRoute(mission->route());

    // Finaly save mission
    if (m_missions.contains(mission->id))
    {
        m_missionsRepo->update(mission);
        m_homeItemsRepo->update(mission->home);

        emit missionChanged(mission);
    }
    else
    {
        m_missionsRepo->insert(mission);
        m_homeItemsRepo->insert(mission->home, mission->id);

        m_missions.insert(mission->id, mission);
        emit missionAdded(mission);
    }
}

Mission* MissionsService::readMission(const QVariant& id)
{
    QVariantMap map = m_missionsRepo->select(id);
    QString typeId = map.value(props::type).toString();

    const MissionType* const type = m_missionTypes.value(typeId);
    if (!type)
    {
        qWarning() << "Unknown mission type" << typeId;
        return nullptr;
    }

    // Read home id for mission
    map[props::home] = m_homeItemsRepo->selectMissionItemId(id);

    Mission* mission = new Mission(type, map);
    m_homeItemsRepo->read(mission->home);

    m_missions.insert(id, mission);
    emit missionAdded(mission);

    Route* route = m_routes->route(map.value(props::route));
    mission->assignRoute(route);

    return mission;
}
