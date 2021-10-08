#ifndef I_MISSIONS_SERVICE_H
#define I_MISSIONS_SERVICE_H

#include "i_service.h"
#include "mission.h"
#include "mission_status.h"

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
    virtual MissionStatus status(const QVariant& id) const = 0;
    virtual QVariantList missionIds() const = 0;
    virtual QList<Mission*> missions() const = 0;
    virtual QList<const MissionType*> missionTypes() const = 0;

    virtual void registerMissionType(const MissionType* type) = 0;
    virtual void unregisterMissionType(const MissionType* type) = 0;

public slots:
    virtual void readAllMissions() = 0;
    virtual void createMission(const QString& type) = 0;
    virtual void removeMission(Mission* mission) = 0;
    virtual void restoreMission(Mission* mission) = 0;
    virtual void saveMission(Mission* mission) = 0;
    virtual void updateStatus(const QVariant& missionId, const MissionStatus& status) = 0;

signals:
    void missionTypesChanged();

    void missionAdded(Mission* mission);
    void missionChanged(Mission* mission);
    void missionRemoved(Mission* mission);
    void statusUpdated(QVariant missionId, MissionStatus status);

    void upload(Mission* mission);   // To the vehicle
    void download(Mission* mission); // From the vehicle
    void cancel(Mission* mission);   // Downloading or uploading
};
} // namespace md::domain

#endif // I_MISSIONS_SERVICE_H
