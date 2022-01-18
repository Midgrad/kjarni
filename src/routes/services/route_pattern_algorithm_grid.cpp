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
    const int spacing = this->parameters().value(route::spacing.id).toInt();
    const float altitude = this->parameters().value(route::altitude.id).toFloat();
    const float heading = this->parameters().value(route::heading.id).toFloat();
    const bool doubled = this->parameters().value(route::doubled.id).toBool();

    // Bounding rect & center of area
    const CartesianRect boundingRect = this->area().boundingRect();
    const Cartesian center = boundingRect.center();

    // Take the rect guaranteed covered survey area
    const double minX = center.x() - boundingRect.diagonal() / 2;
    const double maxX = center.x() + boundingRect.diagonal() / 2;
    const double maxY = center.y() + boundingRect.diagonal() / 2;
    double y = center.y() - boundingRect.diagonal() / 2;

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
    m_path = pathPositions;
}
