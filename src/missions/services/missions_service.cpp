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

RoutePattern* MissionsService::createRoutePattern(const QString& routePatternId)
{
    IRoutePatternFactory* factory = m_patternFactories.value(routePatternId, nullptr);
    if (!factory)
        return nullptr;

    return factory->create(routePatternId);
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

void MissionsService::registerRoutePatternFactory(const QString& routePatternId,
                                                  IRoutePatternFactory* factory)
{
    m_patternFactories[routePatternId] = factory;
}

void MissionsService::unregisterRoutePatternFactory(const QString& routePatternId)
{
    m_patternFactories.remove(routePatternId);
}

void MissionsService::addMission(Mission* mission)
{
    m_missions.insert(mission->id(), mission);
    mission->setParent(this);
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
    this->removeItems(m_itemsRepo->selectMissionRouteItemIds(mission->route()->id));

    // Remove mission
    m_missionsRepo->remove(mission);
    m_missions.remove(mission->id);

    emit missionRemoved(mission);
    mission->deleteLater();
}

void MissionsService::restoreMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);

    QVariantList itemIds = m_itemsRepo->selectMissionRouteItemIds(mission->route()->id);
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
    for (const QVariant& itemId : itemIds)
    {
        auto item = this->readItem(itemId);
        if (item)
            mission->route()->addItem(item); // TODO: valid index
    }

    // Restore mission
    m_missionsRepo->read(mission);

    emit missionChanged(mission);
}

void MissionsService::saveMission(Mission* mission)
{
    QMutexLocker locker(&m_mutex);
    bool added;

    // Update or insert route
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
    QVariantList itemIds = m_itemsRepo->selectMissionRouteItemIds(mission->route()->id);
    for (MissionRouteItem* item : mission->route()->items())
    {
        this->saveItemImpl(item, mission->route()->id, itemIds);
        itemIds.removeOne(item->id);
    }

    // Delete removed items
    this->removeItems(itemIds);

    added ? emit missionAdded(mission) : emit missionChanged(mission);
}

void MissionsService::saveItem(MissionRoute* route, MissionRouteItem* item)
{
    QMutexLocker locker(&m_mutex);

    this->saveItemImpl(item, route->id, m_itemsRepo->selectMissionRouteItemIds(route->id));
}

void MissionsService::restoreItem(MissionRoute* route, MissionRouteItem* item)
{
    Q_UNUSED(route)

    QMutexLocker locker(&m_mutex);

    this->restoreItemImpl(item);
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

    // Create mission
    Mission* mission = new Mission(type, map);
    m_missions.insert(id, mission);

    // Read items for route
    for (const QVariant& itemId : m_itemsRepo->selectMissionRouteItemIds(id))
    {
        auto item = this->readItem(itemId);
        if (item)
            mission->route()->addItem(item);
    }

    emit missionAdded(mission);
    return mission;
}

MissionRouteItem* MissionsService::readItem(const QVariant& id)
{
    QVariantMap select = m_itemsRepo->select(id);
    QString itemTypeId = select.value(props::type).toString();
    const MissionItemType* itemType = nullptr;
    for (const MissionType* routeType : qAsConst(m_missionTypes))
    {
        itemType = routeType->itemType(itemTypeId);
        if (itemType)
            break;
    }

    if (!itemType)
    {
        qWarning() << "Unknown route item type" << itemTypeId;
        return nullptr;
    }

    // Read current item
    return new MissionRouteItem(itemType, select);
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
