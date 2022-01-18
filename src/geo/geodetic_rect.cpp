#include "geodetic_rect.h"

using namespace md::domain;

GeodeticRect::GeodeticRect(const Geodetic& topLeft, const Geodetic& bottomRight) :
    topLeft(topLeft),
    bottomRight(bottomRight)
{
}

Geodetic GeodeticRect::topRight() const
{
    return Geodetic(this->topLeft().latitude, this->bottomRight().longitude,
                    this->topLeft().altitude);
}

Geodetic GeodeticRect::bottomLeft() const
{
    return Geodetic(this->bottomRight().latitude, this->topLeft().longitude,
                    this->bottomRight().altitude);
}

double GeodeticRect::width() const
{
    return this->topLeft().distanceTo(this->topRight());
}

double GeodeticRect::height() const
{
    return this->topLeft().distanceTo(this->bottomLeft());
}

Geodetic GeodeticRect::center() const
{
    return this->topLeft().midPoint(bottomRight);
}

CartesianRect GeodeticRect::nedRect(const Geodetic& origin)
{
    return CartesianRect(this->topLeft().nedPoint(origin), this->bottomRight().nedPoint(origin));
}
