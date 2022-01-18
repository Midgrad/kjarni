#include "route_pattern_algorithm_grid.h"

#include "route_traits.h"

using namespace md::domain;

QVector<Cartesian> RoutePatternAlgorithmGrid::calculate(const QVector<Cartesian>& area,
                                                        const QVariantMap& parameters)
{
    QVector<Cartesian> path;
    // Params
    const float heading = parameters.value(route::heading.id, route::heading.defaultValue).toFloat();
    const bool doubled = parameters.value(route::doubled.id, route::doubled.defaultValue).toBool();
    const int spacing = parameters.value(route::spacing.id, route::spacing.defaultValue).toInt();
    const float altitude = parameters.value(route::altitude.id, route::altitude.defaultValue)
                               .toFloat();

    path += this->trace(heading, altitude, spacing, area);
    // Repeat rotated 90 if doubled
    if (doubled)
        path += this->trace(heading + 90, altitude, spacing, area);

    return path;
}

QVector<Cartesian> RoutePatternAlgorithmGrid::trace(float heading, float altitude, float spacing,
                                                    const CartesianPath& area)
{
    // Bounding rect & center of area
    const CartesianRect boundingRect = area.boundingRect();
    const Cartesian center = boundingRect.center();

    // Trace the rect guaranteed covered survey area
    const double dDiv2 = boundingRect.diagonal() / 2;
    const double minX = center.x() - dDiv2;
    const double maxX = center.x() + dDiv2;
    const double minY = center.y() - dDiv2;
    const double maxY = center.y() + dDiv2;

    double y = minY;
    // Cast lines to detect intersections with area
    QVector<Cartesian> pathPositions;
    for (int i = 0;; ++i)
    {
        Cartesian castPoint = Cartesian(minX, y, -altitude).rotated(heading, center);
        CartesianLine cast(castPoint, Cartesian(maxX, y, -altitude).rotated(heading, center));
        QVector<Cartesian> intersections = CartesianPath(area.intersections2D(cast, true))
                                               .sortedByDistance(castPoint);

        if (intersections.count() >= 2)
        {
            if (i % 2)
            {
                std::reverse(intersections.begin(), intersections.end());
            }
            pathPositions += intersections;
        }
        y += spacing;
        if (y >= maxY)
            break;
    }
    return pathPositions;
}
