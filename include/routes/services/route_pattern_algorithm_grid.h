#ifndef ROUTE_PATTERN_ALGORITHM_GRID_H
#define ROUTE_PATTERN_ALGORITHM_GRID_H

#include "i_route_pattern_algorithm.h"

namespace md::domain
{
class RoutePatternAlgorithmGrid : public IRoutePatternAlgorithm
{
public:
    RoutePatternAlgorithmGrid(const QVector<Cartesian>& area, const QVariantMap& parameters);

    void calculate() override;
    void trace(float heading, const CartesianRect& boundingRect);
};
} // namespace md::domain

#endif // ROUTE_PATTERN_ALGORITHM_GRID_H
