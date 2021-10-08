#include "missions_service.h"

#include <QDebug>

#include "utils.h"

using namespace md::domain;

MissionsService::MissionsService(data_source::IJsonGateway* repository, QObject* parent) :
    IMissionsService(parent),
    m_repository(repository)
{
    m_repository->setParent(this);
}

Mission* MissionsService::mission(const QVariant& id) const
{
    return m_missions.value(id, nullptr);
}

MissionStatus MissionsService::status(const QVariant& id) const
{
    return m_statuses.value(id);
}

QVariantList MissionsService::missionIds() const
{
    return m_missions.keys();
}

QList<Mission*> MissionsService::missions() const
{
    return m_missions.values();
}

QList<const MissionType*> MissionsService::missionTypes() const
{
    return m_missionTypes;
}

void MissionsService::registerMissionType(const MissionType* type)
{
    if (m_missionTypes.contains(type))
        return;

    m_missionTypes.append(type);
    emit missionTypesChanged();
}

void MissionsService::unregisterMissionType(const MissionType* type)
{
    if (!m_missionTypes.contains(type))
        return;

    m_missionTypes.removeOne(type);
    emit missionTypesChanged();
}

void MissionsService::readAllMissions()
{
    for (const QVariant& id : m_repository->selectIds())
    {
        QJsonObject json = m_repository->read(id);
        // Verify id, remove when repository
        json[params::id] = id.toString();

        if (m_missions.contains(id))
        {
            m_missions.value(id)->fromJson(json);
        }
        else
        {
            Mission* mission = new Mission(json, this);
            m_missions.insert(id, mission);
            emit missionAdded(mission);
        }
    }
}

void MissionsService::createMission(const QString& type)
{
    QStringList names;
    for (Mission* mission : qAsConst(m_missions))
    {
        names += mission->name();
    }
    QString name = utils::nameFromType(type, names);

    Mission* mission = new Mission(type, utils::nameToId(name), name, this);
    this->saveMission(mission);
}

void MissionsService::removeMission(Mission* mission)
{
    if (!mission->id().isNull())
        m_repository->remove(mission->id());

    m_missions.remove(mission->id());
    m_statuses.remove(mission->id());

    emit missionRemoved(mission);
    mission->deleteLater();
}

void MissionsService::restoreMission(Mission* mission)
{
    if (mission->id().isNull())
        return;

    QJsonObject json = m_repository->read(mission->id());
    mission->fromJson(json);
    emit missionChanged(mission);
}

void MissionsService::saveMission(Mission* mission)
{
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
        mission->setParent(this);
        emit missionAdded(mission);
    }
}

void MissionsService::updateStatus(const QVariant& missionId, const MissionStatus& status)
{
    if (m_statuses.value(missionId) == status)
        return;

    m_statuses[missionId] = status;
    emit statusUpdated(missionId, status);
}
