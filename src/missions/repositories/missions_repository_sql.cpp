#include "missions_repository_sql.h"

#include <QDebug>

#include "mission_traits.h"

namespace
{
constexpr char missions[] = "missions";
} // namespace

using namespace md::data_source;

MissionsRepositorySql::MissionsRepositorySql(QSqlDatabase* database) :
    domain::IMissionsRepository(),
    m_missionsTable(database, ::missions)
{
}

QVariantMap MissionsRepositorySql::select(const QVariant& missionId)
{
    return m_missionsTable.selectById(missionId);
}

QVariantList MissionsRepositorySql::selectMissionIds()
{
    return m_missionsTable.selectIds();
}

QVariant MissionsRepositorySql::selectMissionIdForVehicle(const QVariant& vehicleId)
{
    auto select = m_missionsTable.selectOne({ { domain::props::vehicle, vehicleId } },
                                            domain::props::id);
    if (select.isEmpty())
        return QVariant();

    return select.first();
}

void MissionsRepositorySql::insert(domain::Mission* mission)
{
    m_missionsTable.insertEntity(mission);
}

void MissionsRepositorySql::read(domain::Mission* mission)
{
    m_missionsTable.readEntity(mission);
}

void MissionsRepositorySql::update(domain::Mission* mission)
{
    m_missionsTable.updateEntity(mission);
}

void MissionsRepositorySql::remove(domain::Mission* mission)
{
    m_missionsTable.removeEntity(mission);
}
