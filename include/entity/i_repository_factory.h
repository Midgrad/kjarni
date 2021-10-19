#ifndef I_REPOSITORY_FACTORY_H
#define I_REPOSITORY_FACTORY_H

#include "i_entity_repository.h"

namespace md
{
namespace domain
{
class IRepositoryFactory
{
public:
    IRepositoryFactory() = default;
    virtual ~IRepositoryFactory() = default;

    virtual IEntityRepository* create(const QString& context) = 0;
};

} // namespace domain
} // namespace md

#endif // I_REPOSITORY_FACTORY_H
