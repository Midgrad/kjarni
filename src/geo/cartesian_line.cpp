#include "cartesian_line.h"

#include <QtMath>

using namespace md::domain;

CartesianLine::CartesianLine(const Cartesian& p1, const Cartesian& p2) : p1(p1), p2(p2)
{
}

double CartesianLine::length() const
{
    return this->p1().distanceTo(this->p2());
}

Cartesian CartesianLine::center() const
{
    return this->p1().midPoint(p2);
}

Cartesian CartesianLine::intersection2d(const CartesianLine& other) const
{
    const double dx1 = this->p2().x - this->p1().x;
    const double dy1 = this->p2().y - this->p1().y;
    const double dx2 = other.p2().x - other.p1().x;
    const double dy2 = other.p2().y - other.p1().y;

    const double s = (-dy1 * (this->p1().x - other.p1().x) + dx1 * (this->p1().y - other.p1().y)) /
                     (-dx2 * dy1 + dx1 * dy2);
    const double t = (dx2 * (this->p1().y - other.p1().y) - dy2 * (this->p1().x - other.p1().x)) /
                     (-dx2 * dy1 + dx1 * dy2);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        return Cartesian(this->p1().x + (t * dx1), this->p1().y + (t * dy1), other.p1().z);
    }

    return Cartesian();
}
