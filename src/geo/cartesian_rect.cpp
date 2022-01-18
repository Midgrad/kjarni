#include "cartesian_rect.h"

#include <QtMath>

using namespace md::domain;

CartesianRect::CartesianRect(const Cartesian& topLeft, const Cartesian& bottomRight) :
    topLeft(topLeft),
    bottomRight(bottomRight)
{
}

CartesianRect::CartesianRect(const Cartesian& center, double width, double height) :
    topLeft(center.translated(Cartesian(-width / 2, -height / 2, 0))),
    bottomRight(center.translated(Cartesian(width / 2, height / 2, 0)))
{
}

Cartesian CartesianRect::topRight() const
{
    return Cartesian(this->bottomRight().x, this->topLeft().y, this->topLeft().z);
}

Cartesian CartesianRect::bottomLeft() const
{
    return Cartesian(this->topLeft().x, this->bottomRight().y, this->bottomRight().z);
}

double CartesianRect::width() const
{
    return this->topLeft().distanceTo(this->topRight());
}

double CartesianRect::height() const
{
    return this->bottomRight().distanceTo(this->bottomLeft());
}

double CartesianRect::diagonal() const
{
    return this->topLeft().distanceTo(this->bottomRight());
}

Cartesian CartesianRect::center() const
{
    return this->topLeft().midPoint(bottomRight);
}
