#ifndef I_COMM_LINK_TYPES_REPOSITORY_H
#define I_COMM_LINK_TYPES_REPOSITORY_H

#include "comm_link_type.h"

namespace md::domain
{
class ICommLinkTypesRepository
{
public:
    ICommLinkTypesRepository() = default;
    virtual ~ICommLinkTypesRepository() = default;

    virtual QList<const CommLinkType*> selectCommLinkTypes() = 0;
};
} // namespace md::domain

#endif // I_COMM_LINK_TYPES_REPOSITORY_H
