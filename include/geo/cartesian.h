#ifndef CARTESIAN_H
#define CARTESIAN_H

#include "geo_traits.h"

namespace md::domain
{
class Cartesian
{
public:
    Cartesian(double x, double y, float z);

    const double x;
    const double y;
    const float z;
};
} // namespace md::domain

#endif // CARTESIAN_H
