#include "comm_links_repository_sql.h"

#include <QDebug>

namespace
{
constexpr char comm_links[] = "comm_links";
} // namespace

using namespace md::data_source;

CommLinksRepositorySql::CommLinksRepositorySql(QSqlDatabase* database) :
    ICommLinksRepository(),
    m_commLinksTable(database, ::comm_links, { domain::props::params })
{
}

QVariantList CommLinksRepositorySql::selectCommLinkIds()
{
    return m_commLinksTable.selectIds();
}

QVariantMap CommLinksRepositorySql::select(const QVariant& comm_linkId)
{
    return m_commLinksTable.selectById(comm_linkId);
}

void CommLinksRepositorySql::insert(domain::CommLink* comm_link)
{
    m_commLinksTable.insertEntity(comm_link);
}

void CommLinksRepositorySql::read(domain::CommLink* comm_link)
{
    m_commLinksTable.readEntity(comm_link);
}

void CommLinksRepositorySql::update(domain::CommLink* comm_link)
{
    m_commLinksTable.updateEntity(comm_link);
}

void CommLinksRepositorySql::remove(domain::CommLink* comm_link)
{
    m_commLinksTable.removeEntity(comm_link);
}
