#ifndef I_ROUTE_PATTERN_ALGORITHM_H
#define I_ROUTE_PATTERN_ALGORITHM_H

#include "cartesian_path.h"

namespace md::domain
{
class IRoutePatternAlgorithm
{
public:
    virtual ~IRoutePatternAlgorithm() = default;

    virtual QVector<Cartesian> calculate(const QVector<Cartesian>& area,
                                         const QVariantMap& parameters) = 0;
};
} // namespace md::domain

#endif // I_ROUTE_PATTERN_ALGORITHM_H
