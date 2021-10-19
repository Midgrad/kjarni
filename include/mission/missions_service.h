#ifndef MISSIONS_SERVICE_H
#define MISSIONS_SERVICE_H

#include "i_missions_service.h"
#include "i_repository_factory.h"

#include <QMutex>

namespace md::domain
{
class MissionsService : public IMissionsService
{
    Q_OBJECT

public:
    explicit MissionsService(IRepositoryFactory* repoFactory, QObject* parent = nullptr);
    ~MissionsService() override;

    Mission* mission(const QVariant& id) const override;
    Mission* missionForVehicle(const QString& vehicleId) const override;
    QVariantList missionIds() const override;
    QList<Mission*> missions() const override;
    QList<const MissionType*> missionTypes() const override;

    void registerMissionType(const MissionType* type) override;
    void unregisterMissionType(const MissionType* type) override;

public slots:
    void readAll() override;
    void removeMission(Mission* mission) override;
    void restoreMission(Mission* mission) override;
    void saveMission(Mission* mission) override;

private:
    const QScopedPointer<IEntityRepository> m_missionRepo;
    const QScopedPointer<IEntityRepository> m_routesRepo;
    const QScopedPointer<IEntityRepository> m_waypointsRepo;
    QMap<QString, const MissionType*> m_missionTypes;
    QMap<QVariant, Mission*> m_missions; // Move map to repos,

    mutable QMutex m_mutex;
};
} // namespace md::domain

#endif // MISSIONS_SERVICE_H
