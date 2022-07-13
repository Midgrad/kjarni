#ifndef I_COMM_LINKS_REPOSITORY_H
#define I_COMM_LINKS_REPOSITORY_H

#include "comm_link.h"

namespace md::domain
{
class ICommLinksRepository
{
public:
    ICommLinksRepository() = default;
    virtual ~ICommLinksRepository() = default;

    virtual QVariantList selectCommLinkIds() = 0;
    virtual QVariantMap select(const QVariant& commLinkId) = 0;

    virtual void insert(CommLink* commLink) = 0;
    virtual void read(CommLink* commLink) = 0;
    virtual void update(CommLink* commLink) = 0;
    virtual void remove(CommLink* commLink) = 0;
};
} // namespace md::domain

#endif // I_COMM_LINKS_REPOSITORY_H
