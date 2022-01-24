#ifndef I_COMMUNICATION_DESCRIPTION_REPOSITORY_H
#define I_COMMUNICATION_DESCRIPTION_REPOSITORY_H

#include "communication_description.h"

namespace md::domain
{
class ICommunicationDescriptionRepository
{
public:
    ICommunicationDescriptionRepository() = default;
    virtual ~ICommunicationDescriptionRepository() = default;

    virtual QVariantList selectDescriptionIds() = 0;
    virtual QVariantMap select(const QVariant& descriptionId) = 0;

    virtual void insert(CommunicationDescription* description) = 0;
    virtual void read(CommunicationDescription* description) = 0;
    virtual void update(CommunicationDescription* description) = 0;
    virtual void remove(CommunicationDescription* description) = 0;
};
} // namespace md::domain

#endif // I_COMMUNICATION_DESCRIPTION_REPOSITORY_H
