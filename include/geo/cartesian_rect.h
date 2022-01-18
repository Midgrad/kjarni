#ifndef CARTESIAN_RECT_H
#define CARTESIAN_RECT_H

#include "cartesian.h"

namespace md::domain
{
class CartesianRect
{
public:
    CartesianRect(const Cartesian& topLeft = Cartesian(),
                  const Cartesian& bottomRight = Cartesian());
    CartesianRect(const Cartesian& center, double width, double height);

    utils::ConstProperty<Cartesian> topLeft;
    utils::ConstProperty<Cartesian> bottomRight;

    Cartesian topRight() const;
    Cartesian bottomLeft() const;

    double width() const;
    double height() const;
    double diagonal() const;
    Cartesian center() const;
};

} // namespace md::domain

#endif // CARTESIAN_RECT_H
