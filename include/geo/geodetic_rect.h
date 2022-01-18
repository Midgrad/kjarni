#ifndef GEODETIC_RECT_H
#define GEODETIC_RECT_H

#include "cartesian_rect.h"
#include "geodetic.h"

namespace md::domain
{
class GeodeticRect
{
public:
    GeodeticRect(const Geodetic& topLeft = Geodetic(), const Geodetic& bottomRight = Geodetic());

    utils::ConstProperty<Geodetic> topLeft;
    utils::ConstProperty<Geodetic> bottomRight;

    Geodetic topRight() const;
    Geodetic bottomLeft() const;

    double width() const;
    double height() const;
    Geodetic center() const;

    CartesianRect nedRect(const Geodetic& origin);
};

} // namespace md::domain

#endif // GEODETIC_RECT_H
