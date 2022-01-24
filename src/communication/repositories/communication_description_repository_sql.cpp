#include "communication_description_repository_sql.h"

#include <QDebug>

namespace
{
constexpr char communicationDescriptions[] = "communication_descriptions";
} // namespace

using namespace md::data_source;

CommunicationDescriptionRepositorySql::CommunicationDescriptionRepositorySql(QSqlDatabase* database) :
    domain::ICommunicationDescriptionRepository(),
    m_communicationDescriptionTable(database, ::communicationDescriptions, { domain::props::params })
{
}

QVariantList CommunicationDescriptionRepositorySql::selectDescriptionIds()
{
    return m_communicationDescriptionTable.selectIds();
}

QVariantMap CommunicationDescriptionRepositorySql::select(const QVariant& descriptionId)
{
    return m_communicationDescriptionTable.selectById(descriptionId);
}

void CommunicationDescriptionRepositorySql::insert(domain::CommunicationDescription* description)
{
    m_communicationDescriptionTable.insertEntity(description);
}

void CommunicationDescriptionRepositorySql::read(domain::CommunicationDescription* description)
{
    m_communicationDescriptionTable.readEntity(description);
}

void CommunicationDescriptionRepositorySql::update(domain::CommunicationDescription* description)
{
    m_communicationDescriptionTable.updateEntity(description);
}

void CommunicationDescriptionRepositorySql::remove(domain::CommunicationDescription* description)
{
    m_communicationDescriptionTable.removeEntity(description);
}
