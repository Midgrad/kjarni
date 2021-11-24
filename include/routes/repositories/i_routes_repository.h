#ifndef I_ROUTES_REPOSITORY_H
#define I_ROUTES_REPOSITORY_H

#include "route.h"

namespace md::domain
{
class IRoutesRepository
{
public:
    IRoutesRepository() = default;
    virtual ~IRoutesRepository() = default;

    virtual QVariantMap select(const QVariant& routeId) = 0;
    virtual QVariantList selectRouteIds() = 0;

    virtual void insert(Route* route) = 0;
    virtual void read(Route* route) = 0;
    virtual void update(Route* route) = 0;
    virtual void remove(Route* route) = 0;
};
} // namespace md::domain

#endif // I_ROUTES_REPOSITORY_H
