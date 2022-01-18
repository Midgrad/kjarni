#include "route_pattern_algorithm_snail.h"

#include "route_traits.h"

using namespace md::domain;

QVector<Cartesian> RoutePatternAlgorithmSnail::calculate(const QVector<Cartesian>& area,
                                                         const QVariantMap& parameters)
{
    // Params
    const float heading = parameters.value(route::heading.id, route::heading.defaultValue).toFloat();
    const int spacing = parameters.value(route::spacing.id, route::spacing.defaultValue).toInt();
    const float altitude = parameters.value(route::altitude.id, route::altitude.defaultValue)
                               .toFloat();

    return this->trace(heading, altitude, spacing, area);
}

QVector<Cartesian> RoutePatternAlgorithmSnail::trace(float heading, float altitude, float spacing,
                                                     const CartesianPath& area)
{
    const Cartesian center = area.center();
    const double limit = area.sortedByDistance(center).first().distanceTo(center) * 2;

    Cartesian current(center.x, center.y, -altitude);
    double distance = spacing;
    QVector<Cartesian> pathPositions;
    pathPositions += current;

    while (distance < limit)
    {
        current = current.translated(Cartesian(distance, 0, 0)).rotated(heading, current);
        heading += 90;
        if (heading >= 360)
            heading -= 360;
        distance += spacing;

        pathPositions += current;
    }
    return pathPositions;
}
