#ifndef I_ENTITY_REPOSITORY_H
#define I_ENTITY_REPOSITORY_H

#include <QVariant>

namespace md
{
namespace data_source
{
template<class Entity>
class IEntityRepository
{
public:
    IEntityRepository() = default;
    virtual ~IEntityRepository() = default;

    virtual QVariantList selectIds() const = 0;

    virtual void create(Entity* entity) = 0; // Create in repository
    virtual void read(Entity* entity) = 0;   // Read from repository
    virtual void update(Entity* entity) = 0; // Update in repository
    virtual void remove(Entity* entity) = 0; // Delete in repository
};

} // namespace data_source
} // namespace md

#endif // I_ENTITY_REPOSITORY_H
