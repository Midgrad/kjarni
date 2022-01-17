#ifndef CARTESIAN_LINE_H
#define CARTESIAN_LINE_H

#include "cartesian.h"

namespace md::domain
{
class CartesianLine
{
public:
    CartesianLine(const Cartesian& p1 = Cartesian(), const Cartesian& p2 = Cartesian());

    utils::ConstProperty<Cartesian> p1;
    utils::ConstProperty<Cartesian> p2;

    double length() const;
    Cartesian center() const;
    Cartesian intersection2d(const CartesianLine& other) const;
};

} // namespace md::domain

#endif // CARTESIAN_LINE_H
