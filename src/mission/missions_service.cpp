#include "missions_service.h"

#include <QDebug>

#include "utils.h"

namespace
{
constexpr char missions[] = "missions";
} // namespace

using namespace md::domain;

MissionsService::MissionsService(IRoutesService* routes, IRepositoryFactory* repoFactory,
                                 QObject* parent) :
    IMissionsService(parent),
    m_routes(routes),
    m_missionRepo(repoFactory->create(::missions))
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

    // TODO: remove all-caching and switch to querry
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
    m_routes->registerRouteType(type->routeType);

    emit missionTypesChanged();
}

void MissionsService::unregisterMissionType(const MissionType* type)
{
    QMutexLocker locker(&m_mutex);

    if (!m_missionTypes.contains(type->name))
        return;

    m_routes->unregisterRouteType(type->routeType);
    m_missionTypes.remove(type->name);

    emit missionTypesChanged();
}

void MissionsService::readAll()
{
    QMutexLocker locker(&m_mutex);

    m_routes->readAll();

    for (const QVariant& missionId : m_missionRepo->selectIds())
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

    if (mission->route())
        m_routes->saveRoute(mission->route());

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

Mission* MissionsService::readMission(const QVariant& id)
{
    QVariantMap map = m_missionRepo->select(id);
    QString typeName = map.value(params::type).toString();

    const MissionType* const type = m_missionTypes.value(typeName);
    if (!type)
    {
        qWarning() << "Unknown mission type" << typeName;
        return nullptr;
    }

    Mission* mission = new Mission(map, type, this);
    m_missions.insert(id, mission);
    emit missionAdded(mission);

    QVariant routeId = map.value(params::route);

    return mission;
}
