#ifndef ROUTE_PATTERN_ALGORITHM_SNAIL_H
#define ROUTE_PATTERN_ALGORITHM_SNAIL_H

#include "i_route_pattern_algorithm.h"

namespace md::domain
{
class RoutePatternAlgorithmSnail : public IRoutePatternAlgorithm
{
public:
    RoutePatternAlgorithmSnail(const QVector<Cartesian>& area, const QVariantMap& parameters);

    void calculate() override;

private:
    void trace(float heading, float altitude, float spacing, const CartesianPath& area);
};
} // namespace md::domain

#endif // ROUTE_PATTERN_ALGORITHM_SNAIL_H
