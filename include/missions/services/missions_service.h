#ifndef MISSIONS_SERVICE_H
#define MISSIONS_SERVICE_H

#include "i_mission_items_repository.h"
#include "i_missions_repository.h"
#include "i_missions_service.h"

#include <QMutex>

namespace md::domain
{
class MissionsService : public IMissionsService
{
    Q_OBJECT

public:
    MissionsService(IMissionsRepository* missionsRepo, IMissionItemsRepository* itemsRepo,
                    QObject* parent = nullptr);

    Mission* mission(const QVariant& id) const override;
    Mission* missionForVehicle(const QVariant& vehicleId) const override;
    QVariantList missionIds() const override;
    QList<Mission*> missions() const override;
    MissionOperation* operationForMission(Mission* mission) const override;
    IMissionsFactory* missionFactory(const QString& type) const override;
    QStringList missionTypes() const override;

    void startOperation(Mission* mission, MissionOperation::Type type) override;
    void endOperation(MissionOperation* operation, MissionOperation::State state) override;

    void registerMissionFactory(const QString& type, IMissionsFactory* factory) override;
    void unregisterMissionFactory(const QString& type) override;

public slots:
    void readAll() override;
    void removeMission(Mission* mission) override;
    void restoreMission(Mission* mission) override;
    void saveMission(Mission* mission) override;
    void saveItem(Mission* mission, MissionRouteItem* item) override;
    void restoreItem(Mission* mission, MissionRouteItem* item) override;

private:
    Mission* readMission(const QVariant& id);
    MissionRouteItem* readItem(const QVariant& id, IMissionsFactory* factory);
    void saveItemImpl(MissionRouteItem* item, const QVariant& parentId, const QVariantList& itemIds);
    void restoreItemImpl(MissionRouteItem* item);
    void removeItems(const QVariantList& itemsIds);

    IMissionsRepository* const m_missionsRepo;
    IMissionItemsRepository* const m_itemsRepo;

    QMap<QString, IMissionsFactory*> m_missionFactories;
    QMap<QVariant, Mission*> m_missions;
    QMap<Mission*, MissionOperation*> m_operations;

    mutable QMutex m_mutex;
};
} // namespace md::domain

#endif // MISSIONS_SERVICE_H
