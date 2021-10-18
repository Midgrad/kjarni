#include "missions_service.h"

#include <QDebug>

#include "utils.h"

using namespace md::domain;

MissionsService::MissionsService(data_source::IJsonGateway* repository, QObject* parent) :
    IMissionsService(parent),
    m_repository(repository)
{
    qRegisterMetaType<MissionStatus>("MissionStatus");

    m_repository->setParent(this);
}

Mission* MissionsService::mission(const QVariant& id) const
{
    QMutexLocker locker(&m_mutex);
    qDebug() << id;
    return m_missions.value(id, nullptr);
}

Mission* MissionsService::missionForVehicle(const QString& vehicleId) const
{
    QMutexLocker locker(&m_mutex);

    auto result = std::find_if(m_missions.begin(), m_missions.end(), [vehicleId](Mission* mision) {
        return mision->vehicle() == vehicleId;
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

void MissionsService::readAllMissions()
{
    QMutexLocker locker(&m_mutex);

    for (const QVariant& id : m_repository->selectIds())
    {
        QJsonObject json = m_repository->read(id);
        // Verify id, TODO: remove when repository
        json[params::id] = id.toString();

        if (m_missions.contains(id))
        {
            m_missions.value(id)->fromJson(json);
        }
        else
        {
            QString typeName = json.value(params::type).toString();
            auto type = m_missionTypes.value(typeName);
            if (!type)
            {
                qWarning() << "Unknown mission type" << typeName;
                continue;
            }

            Mission* mission = new Mission(json, *type, this);
            m_missions.insert(id, mission);
            emit missionAdded(mission);
        }
    }
}

void MissionsService::removeMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);

    if (!mission->id().isNull())
        m_repository->remove(mission->id());

    m_missions.remove(mission->id());

    emit missionRemoved(mission);
    mission->deleteLater();
}

void MissionsService::restoreMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);

    if (mission->id().isNull())
        return;

    QJsonObject json = m_repository->read(mission->id());
    mission->fromJson(json);
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

    m_repository->save(mission->id(), mission->toJson(true));

    if (m_missions.contains(mission->id()))
    {
        emit missionChanged(mission);
    }
    else
    {
        m_missions[mission->id()] = mission;
        mission->moveToThread(this->thread());
        mission->setParent(this);
        emit missionAdded(mission);
    }
}
