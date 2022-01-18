#include "route_pattern_algorithm_snail.h"

#include "route_traits.h"

using namespace md::domain;

RoutePatternAlgorithmSnail::RoutePatternAlgorithmSnail(const QVector<Cartesian>& area,
                                                       const QVariantMap& parameters) :
    IRoutePatternAlgorithm(area, parameters)
{
}

void RoutePatternAlgorithmSnail::calculate()
{
    // Params
    const float heading =
        this->parameters().value(route::heading.id, route::heading.defaultValue).toFloat();
    const int spacing =
        this->parameters().value(route::spacing.id, route::spacing.defaultValue).toInt();
    const float altitude =
        this->parameters().value(route::altitude.id, route::altitude.defaultValue).toFloat();

    this->trace(heading, altitude, spacing, this->area());
}

void RoutePatternAlgorithmSnail::trace(float heading, float altitude, float spacing,
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
    m_path += pathPositions;
}
