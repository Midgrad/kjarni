#ifndef COMM_LINKS_REPOSITORY_SQL_H
#define COMM_LINKS_REPOSITORY_SQL_H

#include "entity_sql_table.h"
#include "i_comm_links_repository.h"

namespace md::data_source
{
class CommLinksRepositorySql : public domain::ICommLinksRepository
{
public:
    CommLinksRepositorySql(QSqlDatabase* database);

    QVariantList selectCommLinkIds() override;
    QVariantMap select(const QVariant& commLinkId) override;

    void insert(domain::CommLink* commLink) override;
    void read(domain::CommLink* commLink) override;
    void update(domain::CommLink* commLink) override;
    void remove(domain::CommLink* commLink) override;

private:
    EntitySqlTable m_commLinksTable;
};
} // namespace md::data_source

#endif // COMM_LINKS_REPOSITORY_SQL_H
