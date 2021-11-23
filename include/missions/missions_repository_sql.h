#ifndef MISSIONS_REPOSITORY_H
#define MISSIONS_REPOSITORY_H

#include "entity_sql_table.h"
#include "i_missions_repository.h"
#include "i_routes_service.h"

#include <QMutex>

namespace md::domain
{
class MissionsRepositorySql : public IMissionsRepository
{
    Q_OBJECT

public:
    explicit MissionsRepositorySql(IRoutesService* routes, QSqlDatabase* database,
                                   QObject* parent = nullptr);
    ~MissionsRepositorySql() override;

    Mission* mission(const QVariant& id) const override;
    Mission* missionForVehicle(const QVariant& vehicleId) const override;
    QVariantList missionIds() const override;
    QList<Mission*> missions() const override;
    const MissionType* missionType(const QString& id) const override;
    QList<const MissionType*> missionTypes() const override;

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
    data_source::EntitySqlTable m_missionsTable;
    data_source::EntitySqlTable m_waypointsTable;
    data_source::SqlTable m_homeWaypointsTable;

    QMap<QString, const MissionType*> m_missionTypes;
    QMap<QVariant, Mission*> m_missions;

    mutable QMutex m_mutex;
};
} // namespace md::domain

#endif // MISSIONS_REPOSITORY_H
