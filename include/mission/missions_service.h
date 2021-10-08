#ifndef MISSIONS_SERVICE_H
#define MISSIONS_SERVICE_H

#include "i_json_gateway.h"
#include "i_missions_service.h"

namespace md::domain
{
class MissionsService : public IMissionsService
{
    Q_OBJECT

public:
    explicit MissionsService(data_source::IJsonGateway* repository, QObject* parent = nullptr);

    Mission* mission(const QVariant& id) const override;
    MissionStatus status(const QVariant& id) const override;
    QVariantList missionIds() const override;
    QList<Mission*> missions() const override;
    QList<const MissionType*> missionTypes() const override;

    void registerMissionType(const MissionType* type) override;
    void unregisterMissionType(const MissionType* type) override;

public slots:
    void readAllMissions() override;
    void createMission(const QString& type) override;
    void removeMission(Mission* mission) override;
    void restoreMission(Mission* mission) override;
    void saveMission(Mission* mission) override;
    void updateStatus(const QVariant& missionId, const MissionStatus& status) override;

private:
    data_source::IJsonGateway* const m_repository;
    QList<const MissionType*> m_missionTypes;
    QMap<QVariant, Mission*> m_missions;
    QMap<QVariant, MissionStatus> m_statuses;
};
} // namespace md::domain

#endif // MISSIONS_SERVICE_H
