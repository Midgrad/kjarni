#ifndef I_ENTITY_REPOSITORY_H
#define I_ENTITY_REPOSITORY_H

#include "entity.h"

namespace md
{
namespace domain
{
class IEntityRepository
{
public:
    IEntityRepository() = default;
    virtual ~IEntityRepository() = default;

    virtual QVariantList selectIds() const = 0;
    virtual QVariantMap select(const QVariant& id) const = 0;

    virtual void insert(Entity* entity) = 0; // Create in repository
    virtual void read(Entity* entity) = 0;   // Read from repository
    virtual void update(Entity* entity) = 0; // Update in repository
    virtual void remove(Entity* entity) = 0; // Delete in repository
};

} // namespace domain
} // namespace md

#endif // I_ENTITY_REPOSITORY_H
