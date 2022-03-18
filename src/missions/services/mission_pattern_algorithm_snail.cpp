#include "mission_pattern_algorithm_snail.h"

#include "mission_traits.h"

using namespace md::domain;

QVector<Cartesian> RoutePatternAlgorithmSnail::calculate(const QVector<Cartesian>& area,
                                                         const QVariantMap& parameters)
{
    // Params
    const float heading = parameters.value(mission::heading.id, mission::heading.defaultValue).toFloat();
    const int spacing = parameters.value(mission::spacing.id, mission::spacing.defaultValue).toInt();
    const float altitude = parameters.value(mission::altitude.id, mission::altitude.defaultValue)
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
