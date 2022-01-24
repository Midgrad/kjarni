#ifndef COMMUNICATION_DESCRIPTION_REPOSITORY_SQL_H
#define COMMUNICATION_DESCRIPTION_REPOSITORY_SQL_H

#include "entity_sql_table.h"
#include "i_communication_description_repository.h"

namespace md::data_source
{
class CommunicationDescriptionRepositorySql : public domain::ICommunicationDescriptionRepository
{
public:
    CommunicationDescriptionRepositorySql(QSqlDatabase* database);

    QVariantList selectDescriptionIds() override;
    QVariantMap select(const QVariant& descriptionId) override;

    void insert(domain::CommunicationDescription* description) override;
    void read(domain::CommunicationDescription* description) override;
    void update(domain::CommunicationDescription* description) override;
    void remove(domain::CommunicationDescription* description) override;

private:
    EntitySqlTable m_communicationDescriptionTable;
};
} // namespace md::data_source

#endif // COMMUNICATION_DESCRIPTION_REPOSITORY_SQL_H
