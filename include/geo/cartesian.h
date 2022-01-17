#ifndef CARTESIAN_H
#define CARTESIAN_H

#include "magic_property.hpp"

namespace md::domain
{
class Cartesian
{
public:
    Cartesian(double x, double y, float z);
    Cartesian(const QVariantMap& map);
    Cartesian();

    utils::ConstProperty<double> x;
    utils::ConstProperty<double> y;
    utils::ConstProperty<float> z;

    QVariantMap toVariantMap() const;
    bool isNull() const;
    bool isValid() const;
    double distanceTo(const Cartesian& other) const;

    Cartesian translated(const Cartesian& d) const;
    Cartesian rotated(float heading) const;
    Cartesian operator-() const;
};

bool operator==(const Cartesian& first, const Cartesian& second);
} // namespace md::domain

#endif // CARTESIAN_H
