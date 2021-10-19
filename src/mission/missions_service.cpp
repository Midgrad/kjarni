#include "missions_service.h"

#include <QDebug>

#include "utils.h"

namespace
{
constexpr char missions[] = "missions";
constexpr char routes[] = "routes";
constexpr char waypoints[] = "waypoints";
} // namespace

using namespace md::domain;

MissionsService::MissionsService(IRepositoryFactory* repoFactory, QObject* parent) :
    IMissionsService(parent),
    m_missionRepo(repoFactory->create(::missions)),
    m_routesRepo(repoFactory->create(::routes)),
    m_waypointsRepo(repoFactory->create(::waypoints))
{
    qRegisterMetaType<MissionStatus>("MissionStatus");
}

MissionsService::~MissionsService()
{
}

Mission* MissionsService::mission(const QVariant& id) const
{
    QMutexLocker locker(&m_mutex);

    return m_missions.value(id, nullptr);
}

Mission* MissionsService::missionForVehicle(const QString& vehicleId) const
{
    QMutexLocker locker(&m_mutex);

    auto result = std::find_if(m_missions.begin(), m_missions.end(), [vehicleId](Mission* mision) {
        return mision->vehicleId() == vehicleId;
    });
    if (result == m_missions.end())
        return nullptr;

    return result.value();
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

QList<const MissionType*> MissionsService::missionTypes() const
{
    QMutexLocker locker(&m_mutex);
    return m_missionTypes.values();
}

void MissionsService::registerMissionType(const MissionType* type)
{
    QMutexLocker locker(&m_mutex);

    if (m_missionTypes.contains(type->name))
        return;

    m_missionTypes.insert(type->name, type);
    emit missionTypesChanged();
}

void MissionsService::unregisterMissionType(const MissionType* type)
{
    QMutexLocker locker(&m_mutex);

    if (!m_missionTypes.contains(type->name))
        return;

    m_missionTypes.remove(type->name);
    emit missionTypesChanged();
}

void MissionsService::readAll()
{
    QMutexLocker locker(&m_mutex);

    for (const QVariant& id : m_missionRepo->selectIds())
    {
        Mission* mission = nullptr;
        if (m_missions.contains(id))
        {
            mission = m_missions.value(id);
            m_missionRepo->read(m_missions.value(id));
        }
        else
        {
            QVariantMap map = m_missionRepo->select(id);
            QString typeName = map.value(params::type).toString();
            const MissionType* const type = m_missionTypes.value(typeName);
            if (!type)
            {
                qWarning() << "Unknown mission type" << typeName;
                continue;
            }

            mission = new Mission(map, type, this);
            m_missions.insert(id, mission);
            emit missionAdded(mission);
        }
    }
}

void MissionsService::removeMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);

    if (!mission->id().isNull())
        m_missionRepo->remove(mission);

    m_missions.remove(mission->id());

    emit missionRemoved(mission);
    mission->deleteLater();
}

void MissionsService::restoreMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);

    if (mission->id().isNull())
        return;

    m_missionRepo->read(mission);
    emit missionChanged(mission);
}

void MissionsService::saveMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);

    if (mission->id().isNull())
    {
        qWarning() << "Can't save mission with no id" << mission;
        return;
    }

    if (m_missions.contains(mission->id()))
    {
        m_missionRepo->update(mission);
        emit missionChanged(mission);
    }
    else
    {
        m_missionRepo->insert(mission);
        m_missions.insert(mission->id(), mission);
        mission->moveToThread(this->thread());
        mission->setParent(this);
        emit missionAdded(mission);
    }
}
