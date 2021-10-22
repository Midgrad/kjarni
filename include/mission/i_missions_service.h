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
    virtual Mission* missionForVehicle(const QVariant& vehicleId) const = 0;
    virtual QVariantList missionIds() const = 0;
    virtual QList<Mission*> missions() const = 0;
    virtual QList<const MissionType*> missionTypes() const = 0;

    virtual void registerMissionType(const MissionType* type) = 0;
    virtual void unregisterMissionType(const MissionType* type) = 0;

public slots:
    virtual void readAll() = 0;
    virtual void removeMission(Mission* mission) = 0;
    virtual void restoreMission(Mission* mission) = 0;
    virtual void saveMission(Mission* mission) = 0;

signals:
    void missionTypesChanged();

    void missionAdded(Mission* mission);
    void missionChanged(Mission* mission);
    void missionRemoved(Mission* mission);
};
} // namespace md::domain

#endif // I_MISSIONS_SERVICE_H
