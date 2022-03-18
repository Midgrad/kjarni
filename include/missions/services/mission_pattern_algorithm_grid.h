#ifndef ROUTE_PATTERN_ALGORITHM_GRID_H
#define ROUTE_PATTERN_ALGORITHM_GRID_H

#include "i_mission_pattern_algorithm.h"

namespace md::domain
{
class RoutePatternAlgorithmGrid : public IRoutePatternAlgorithm
{
public:
    QVector<Cartesian> calculate(const QVector<Cartesian>& area,
                                 const QVariantMap& parameters) override;

private:
    QVector<Cartesian> trace(float heading, float altitude, float spacing,
                             const CartesianPath &area);
};
} // namespace md::domain

#endif // ROUTE_PATTERN_ALGORITHM_GRID_H
