#ifndef MISSIONS_REPOSITORY_H
#define MISSIONS_REPOSITORY_H

#include "entity_sql_table.h"
#include "i_missions_repository.h"

namespace md::data_source
{
class MissionsRepositorySql : public domain::IMissionsRepository
{
public:
    MissionsRepositorySql(QSqlDatabase* database);

    QVariantMap select(const QVariant& missionId) override;
    QVariantList selectMissionIds() override;
    QVariant selectMissionIdForVehicle(const QVariant& vehicleId) override;

    void insert(domain::Mission* mission) override;
    void read(domain::Mission* mission) override;
    void update(domain::Mission* mission) override;
    void remove(domain::Mission* mission) override;

private:
    EntitySqlTable m_missionsTable;
};
} // namespace md::data_source

#endif // MISSIONS_REPOSITORY_H
