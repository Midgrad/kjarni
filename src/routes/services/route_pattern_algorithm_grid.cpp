#include "route_pattern_algorithm_grid.h"

#include "route_traits.h"

using namespace md::domain;

RoutePatternAlgorithmGrid::RoutePatternAlgorithmGrid(const QVector<Cartesian>& area,
                                                     const QVariantMap& parameters) :
    IRoutePatternAlgorithm(area, parameters)
{
}

void RoutePatternAlgorithmGrid::calculate()
{
    // Params
    const float heading =
        this->parameters().value(route::heading.id, route::heading.defaultValue).toFloat();
    const bool doubled =
        this->parameters().value(route::doubled.id, route::doubled.defaultValue).toBool();
    const int spacing =
        this->parameters().value(route::spacing.id, route::spacing.defaultValue).toInt();
    const float altitude =
        this->parameters().value(route::altitude.id, route::altitude.defaultValue).toFloat();

    // Bounding rect & center of area
    const CartesianRect boundingRect = this->area().boundingRect();

    this->trace(heading, altitude, spacing, boundingRect);
    // Repeat rotated 90 if doubled
    if (doubled)
        this->trace(heading + 90, altitude, spacing, boundingRect);
}

void RoutePatternAlgorithmGrid::trace(float heading, float altitude, float spacing,
                                      const CartesianRect& boundingRect)
{
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
        QVector<Cartesian> intersections = CartesianPath(this->area().intersections2D(cast, true))
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
    m_path += pathPositions;
}
