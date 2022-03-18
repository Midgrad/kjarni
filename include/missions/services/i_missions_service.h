#ifndef I_MISSIONS_SERVICE_H
#define I_MISSIONS_SERVICE_H

#include "i_service.h"
#include "mission.h"
// Mission & operaion to different services
#include "mission_operation.h"
#include "mission_pattern.h"

namespace md::domain
{
class IMissionsService
    : public QObject
    , public IService
{
    Q_OBJECT

public:
    IMissionsService(QObject* parent) : QObject(parent), IService()
    {
    }

    virtual Mission* mission(const QVariant& id) const = 0;
    virtual Mission* missionForVehicle(const QVariant& vehicleId) const = 0;
    virtual QVariantList missionIds() const = 0;
    virtual QList<Mission*> missions() const = 0;
    virtual MissionOperation* operationForMission(Mission* mission) const = 0;
    virtual const MissionType* missionType(const QString& id) const = 0;
    virtual QList<const MissionType*> missionTypes() const = 0;

    virtual RoutePattern* createRoutePattern(const QString& routePatternId) = 0;

    virtual void startOperation(Mission* mission, MissionOperation::Type type) = 0;
    virtual void endOperation(MissionOperation* operation, MissionOperation::State state) = 0;

    // TODO: merge route factories and mission types to mission factories
    virtual void registerMissionType(const MissionType* type) = 0;
    virtual void unregisterMissionType(const MissionType* type) = 0;
    virtual void registerRoutePatternFactory(const QString& routePatternId,
                                             IRoutePatternFactory* factory) = 0;
    virtual void unregisterRoutePatternFactory(const QString& routePatternId) = 0;

public slots:
    virtual void readAll() = 0;
    virtual void removeMission(Mission* mission) = 0;
    virtual void restoreMission(Mission* mission) = 0;
    virtual void saveMission(Mission* mission) = 0;
    virtual void saveItem(MissionRoute* route, MissionRouteItem* item) = 0;
    virtual void restoreItem(MissionRoute* route, MissionRouteItem* item) = 0;

signals:
    void missionTypesChanged();

    void missionAdded(Mission* mission);
    void missionChanged(Mission* mission);
    void missionRemoved(Mission* mission);

    void operationStarted(MissionOperation* operation);
    void operationEnded(MissionOperation* operation);
};
} // namespace md::domain

#endif // I_MISSIONS_SERVICE_H
