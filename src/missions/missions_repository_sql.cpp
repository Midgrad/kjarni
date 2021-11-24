#include "missions_repository_sql.h"

#include <QDebug>

#include "mission_traits.h"
#include "utils.h"

namespace
{
constexpr char missions[] = "missions";
constexpr char waypoints[] = "waypoints";
constexpr char homeWaypoints[] = "home_waypoints";
} // namespace

using namespace md::domain;

MissionsRepositorySql::MissionsRepositorySql(IRoutesService* routes, QSqlDatabase* database,
                                             QObject* parent) :
    IMissionsRepository(parent),
    m_routes(routes),
    m_missionsTable(database, ::missions),
    m_waypointsTable(database, ::waypoints),
    m_homeWaypointsTable(database, ::homeWaypoints)
{
}

MissionsRepositorySql::~MissionsRepositorySql()
{
}

Mission* MissionsRepositorySql::mission(const QVariant& id) const
{
    QMutexLocker locker(&m_mutex);
    return m_missions.value(id, nullptr);
}

Mission* MissionsRepositorySql::missionForVehicle(const QVariant& vehicleId) const
{
    QMutexLocker locker(&m_mutex);

    // TODO: remove all-caching and switch to querry
    auto result = std::find_if(m_missions.begin(), m_missions.end(), [vehicleId](Mission* mission) {
        return mission->vehicleId() == vehicleId;
    });
    if (result == m_missions.end())
        return nullptr;

    return result.value();
}

QVariantList MissionsRepositorySql::missionIds() const
{
    QMutexLocker locker(&m_mutex);
    return m_missions.keys();
}

QList<Mission*> MissionsRepositorySql::missions() const
{
    QMutexLocker locker(&m_mutex);
    return m_missions.values();
}

const MissionType* MissionsRepositorySql::missionType(const QString& id) const
{
    return m_missionTypes.value(id, nullptr);
}

QList<const MissionType*> MissionsRepositorySql::missionTypes() const
{
    QMutexLocker locker(&m_mutex);
    return m_missionTypes.values();
}

void MissionsRepositorySql::registerMissionType(const MissionType* type)
{
    QMutexLocker locker(&m_mutex);

    if (m_missionTypes.contains(type->id))
        return;

    m_missionTypes.insert(type->id, type);
    m_routes->registerRouteType(type->routeType);

    emit missionTypesChanged();
}

void MissionsRepositorySql::unregisterMissionType(const MissionType* type)
{
    QMutexLocker locker(&m_mutex);

    if (!m_missionTypes.contains(type->id))
        return;

    m_routes->unregisterRouteType(type->routeType);
    m_missionTypes.remove(type->id);

    emit missionTypesChanged();
}

void MissionsRepositorySql::readAll()
{
    QMutexLocker locker(&m_mutex);

    for (const QVariant& missionId : m_missionsTable.selectIds())
    {
        if (!m_missions.contains(missionId))
        {
            this->readMission(missionId);
        }
    }
}

void MissionsRepositorySql::removeMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);

    if (mission->id().isNull())
    {
        qWarning() << "Can't remove mission with no id" << mission;
        return;
    }

    // Remove mission waypoints for route
    m_homeWaypointsTable.removeByCondition({ props::mission, mission->id() });

    // Remove home waypoint
    m_waypointsTable.removeEntity(mission->route()->homePoint());

    // Remove mission
    m_missionsTable.removeEntity(mission);

    m_missions.remove(mission->id());

    emit missionRemoved(mission);
    mission->deleteLater();
}

void MissionsRepositorySql::restoreMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);

    if (mission->id().isNull())
    {
        qWarning() << "Can't resore mission with no id" << mission;
        return;
    }

    m_missionsTable.readEntity(mission);
    m_waypointsTable.readEntity(mission->route()->homePoint());

    emit missionChanged(mission);
}

void MissionsRepositorySql::saveMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);

    if (mission->id().isNull())
    {
        qWarning() << "Can't save mission with no id" << mission;
        return;
    }

    if (mission->route()->route())
        m_routes->saveRoute(mission->route()->route());

    mission->moveToThread(this->thread());
    mission->setParent(this);

    if (m_missions.contains(mission->id()))
    {
        m_waypointsTable.updateEntity(mission->route()->homePoint());
        m_missionsTable.updateEntity(mission);

        emit missionChanged(mission);
    }
    else
    {
        m_waypointsTable.insertEntity(mission->route()->homePoint());
        m_missionsTable.insertEntity(mission);
        m_homeWaypointsTable.insert({ { props::mission, mission->id() },
                                      { props::routeItem, mission->route()->homePoint()->id() } });

        m_missions.insert(mission->id(), mission);
        emit missionAdded(mission);
    }
}

Mission* MissionsRepositorySql::readMission(const QVariant& id)
{
    QVariantMap map = m_missionsTable.selectById(id);
    QString typeId = map.value(props::type).toString();

    const MissionType* const type = m_missionTypes.value(typeId);
    if (!type)
    {
        qWarning() << "Unknown mission type" << typeId;
        return nullptr;
    }

    // Read home waypoint for mission
    QVariantList waypointIds = m_homeWaypointsTable.selectOne({ { props::mission, id } },
                                                              props::routeItem);
    if (waypointIds.length())
    {
        QVariant homeId = waypointIds.first();
        QVariantMap homeMap = m_waypointsTable.selectById(homeId);
        map[props::home] = homeMap;
    }

    Mission* mission = new Mission(type, map);
    m_missions.insert(id, mission);
    emit missionAdded(mission);

    Route* route = m_routes->route(map.value(props::route));
    mission->route()->assignRoute(route);

    return mission;
}
