#ifndef MISSIONS_SERVICE_H
#define MISSIONS_SERVICE_H

#include "i_missions_repository.h"
#include "i_missions_service.h"
#include "i_route_items_repository.h"
#include "i_routes_service.h" // TODO: remove direct dependency

#include <QMutex>

namespace md::domain
{
class MissionsService : public IMissionsService
{
    Q_OBJECT

public:
    MissionsService(IRoutesService* routes, IMissionsRepository* missionsRepo,
                    IRouteItemsRepository* itemsRepo, QObject* parent = nullptr);

    Mission* mission(const QVariant& id) const override;
    Mission* missionForVehicle(const QVariant& vehicleId) const override;
    QVariantList missionIds() const override;
    QList<Mission*> missions() const override;
    MissionOperation* operationForMission(Mission* mission) const override;
    const MissionType* missionType(const QString& id) const override;
    QList<const MissionType*> missionTypes() const override;

    void startOperation(Mission* mission, MissionOperation::Type type) override;
    void endOperation(MissionOperation* operation) override;

    void registerMissionType(const MissionType* type) override;
    void unregisterMissionType(const MissionType* type) override;

public slots:
    void readAll() override;
    void removeMission(Mission* mission) override;
    void restoreMission(Mission* mission) override;
    void saveMission(Mission* mission) override;

private:
    Mission* readMission(const QVariant& id);

    IRoutesService* const m_routes;
    IMissionsRepository* const m_missionsRepo;
    IRouteItemsRepository* const m_itemsRepo;

    QMap<QString, const MissionType*> m_missionTypes;
    QMap<QVariant, Mission*> m_missions;
    QMap<Mission*, MissionOperation*> m_operations;

    mutable QMutex m_mutex;
};
} // namespace md::domain

#endif // MISSIONS_SERVICE_H
