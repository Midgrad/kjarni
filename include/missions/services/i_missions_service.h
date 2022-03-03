#ifndef I_MISSIONS_SERVICE_H
#define I_MISSIONS_SERVICE_H

#include "i_missions_factory.h"
#include "i_service.h"
#include "mission.h"
#include "mission_operation.h"

// TODO: MissionsServiceThreaded
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
    virtual IMissionsFactory* missionFactory(const QString& type) const = 0;
    virtual QStringList missionTypes() const = 0;

    virtual void startOperation(Mission* mission, MissionOperation::Type type) = 0;
    virtual void endOperation(MissionOperation* operation, MissionOperation::State state) = 0;

    virtual void registerMissionFactory(const QString& type, IMissionsFactory* factory) = 0;
    virtual void unregisterMissionFactory(const QString& type) = 0;

public slots:
    virtual void readAll() = 0;
    virtual void removeMission(Mission* mission) = 0;
    virtual void restoreMission(Mission* mission) = 0;
    virtual void saveMission(Mission* mission) = 0;
    virtual void saveItem(Mission* mission, MissionRouteItem* item) = 0;
    virtual void restoreItem(Mission* mission, MissionRouteItem* item) = 0;

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
