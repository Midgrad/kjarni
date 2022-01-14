#include "cartesian.h"

#include <QtMath>

using namespace md::domain;

Cartesian::Cartesian(double x, double y, float z) : x(x), y(y), z(z)
{
}

Cartesian::Cartesian() :
    Cartesian(std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(),
              std::numeric_limits<float>::quiet_NaN())

{
}

bool Cartesian::isNull() const
{
    return qFuzzyIsNull(this->x()) && qFuzzyIsNull(this->y()) && qFuzzyIsNull(this->z());
}

bool Cartesian::isValid() const
{
    return !std::isnan(this->x()) && !std::isnan(this->y()) && !std::isnan(this->z());
}
