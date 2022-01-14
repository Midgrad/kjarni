#ifndef CARTESIAN_H
#define CARTESIAN_H

#include "magic_property.hpp"

namespace md::domain
{
class Cartesian
{
public:
    Cartesian(double x, double y, float z);
    Cartesian();

    bool isNull() const;
    bool isValid() const;

    utils::ConstProperty<double> x;
    utils::ConstProperty<double> y;
    utils::ConstProperty<float> z;
};
} // namespace md::domain

#endif // CARTESIAN_H
