#include "missions_service.h"

#include <QDebug>

#include "mission_traits.h"
#include "utils.h"

using namespace md::domain;

MissionsService::MissionsService(IMissionsRepository* missionsRepo,
                                 IMissionItemsRepository* itemsRepo, QObject* parent) :
    IMissionsService(parent),
    m_missionsRepo(missionsRepo),
    m_itemsRepo(itemsRepo),
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

IMissionsFactory* MissionsService::missionFactory(const QString& type) const
{
    return m_missionFactories.value(type, nullptr);
}

QStringList MissionsService::missionTypes() const
{
    return m_missionFactories.keys();
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

void MissionsService::registerMissionFactory(const QString& type, IMissionsFactory* factory)
{
    QMutexLocker locker(&m_mutex);

    if (m_missionFactories.contains(type))
        return;

    m_missionFactories.insert(type, factory);
    emit missionTypesChanged();
}

void MissionsService::unregisterMissionFactory(const QString& type)
{
    QMutexLocker locker(&m_mutex);

    if (!m_missionFactories.contains(type))
        return;

    m_missionFactories.remove(type);
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

    // Delete items first
    this->removeItems(m_itemsRepo->selectMissionItemsIds(mission->id));

    // Delete mission
    m_missionsRepo->remove(mission);
    m_missions.remove(mission->id);

    emit missionRemoved(mission);
    mission->deleteLater();
}

void MissionsService::restoreMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);

    QVariantList itemIds = m_itemsRepo->selectMissionItemsIds(mission->id);
    for (MissionRouteItem* item : mission->route()->items())
    {
        // Restore stored item
        if (itemIds.contains(item->id))
        {
            this->restoreItemImpl(item);
            itemIds.removeOne(item->id);
        }
        // Remove newbie items
        else
        {
            mission->route()->removeItem(item);
        }
    }

    // Read removed items
    IMissionsFactory* factory = this->missionFactory(mission->type);
    for (const QVariant& itemId : itemIds)
    {
        auto item = this->readItem(itemId, factory);
        if (item)
            mission->route()->addItem(item); // TODO: valid index
    }

    // Finaly restore mission
    m_missionsRepo->read(mission);
    emit missionChanged(mission);
}

void MissionsService::saveMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);
    bool added;

    mission->moveToThread(this->thread());
    mission->setParent(this);

    // Update or insert mission
    if (m_missions.contains(mission->id))
    {
        m_missionsRepo->update(mission);
        added = false;
    }
    else
    {
        m_missionsRepo->insert(mission);
        m_missions.insert(mission->id, mission);

        mission->moveToThread(this->thread());
        mission->setParent(this);
        added = true;
    }

    // Update or insert items
    QVariantList itemIds = m_itemsRepo->selectMissionItemsIds(mission->id);
    for (MissionRouteItem* item : mission->route()->items())
    {
        this->saveItemImpl(item, mission->id, itemIds);
        itemIds.removeOne(mission->id);
    }

    // Delete removed items
    this->removeItems(itemIds);
    added ? emit missionAdded(mission) : emit missionChanged(mission);
}

void MissionsService::saveItem(Mission* mission, MissionRouteItem* item)
{
    QMutexLocker locker(&m_mutex);

    this->saveItemImpl(item, mission->id, m_itemsRepo->selectMissionItemsIds(mission->id));
    emit missionChanged(mission);
}

void MissionsService::restoreItem(Mission* mission, MissionRouteItem* item)
{
    QMutexLocker locker(&m_mutex);

    this->restoreItemImpl(item);
    emit missionChanged(mission);
}

Mission* MissionsService::readMission(const QVariant& id)
{
    QVariantMap select = m_missionsRepo->select(id);
    QString typeId = select.value(props::type).toString();

    IMissionsFactory* factory = this->missionFactory(typeId);
    if (!factory)
    {
        qWarning() << "No mission factory for type" << typeId;
        return nullptr;
    }

    Mission* mission = factory->createMission(select);
    m_missions.insert(id, mission);

    // Read items for route
    for (const QVariant& itemId : m_itemsRepo->selectMissionItemsIds(id))
    {
        auto item = this->readItem(itemId, factory);
        if (item)
            mission->route()->addItem(item);
    }

    emit missionAdded(mission);
    return mission;
}

MissionRouteItem* MissionsService::readItem(const QVariant& id, IMissionsFactory* factory)
{
    QVariantMap select = m_itemsRepo->select(id);
    return factory->createMissionRouteItem(select);
}

void MissionsService::saveItemImpl(MissionRouteItem* item, const QVariant& parentId,
                                   const QVariantList& itemIds)
{
    // Update stored item
    if (itemIds.contains(item->id))
    {
        m_itemsRepo->update(item);
    }
    // Insert newbie items
    else
    {
        m_itemsRepo->insert(item, parentId);
    }
}

void MissionsService::restoreItemImpl(MissionRouteItem* item)
{
    m_itemsRepo->read(item);
}

void MissionsService::removeItems(const QVariantList& itemsIds)
{
    for (const QVariant& itemId : itemsIds)
    {
        m_itemsRepo->removeById(itemId);
    }
}
