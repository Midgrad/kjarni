#include "cartesian.h"

#include <QtMath>

#include "geo_traits.h"

using namespace md::domain;

Cartesian::Cartesian(double x, double y, float z) : x(x), y(y), z(z)
{
}

Cartesian::Cartesian(const QVariantMap& map) :
    Cartesian(map.value(geo::x, qQNaN()).toDouble(), map.value(geo::y, qQNaN()).toDouble(),
              map.value(geo::z, qQNaN()).toFloat())
{
}

Cartesian::Cartesian() :
    Cartesian(std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(),
              std::numeric_limits<float>::quiet_NaN())

{
}

QVariantMap Cartesian::toVariantMap() const
{
    return QVariantMap{ { geo::x, this->x() }, { geo::y, this->y() }, { geo::z, this->z() } };
}

bool Cartesian::isNull() const
{
    return qFuzzyIsNull(this->x()) && qFuzzyIsNull(this->y()) && qFuzzyIsNull(this->z());
}

bool Cartesian::isValid() const
{
    return !std::isnan(this->x()) && !std::isnan(this->y()) && !std::isnan(this->z());
}

double Cartesian::distanceTo(const Cartesian& other) const
{
    return std::hypot(std::hypot(this->x() - other.x(), this->y() - other.y()),
                      this->z() - other.z());
}

Cartesian Cartesian::midPoint(const Cartesian& other) const
{
    return Cartesian((this->x() + other.x()) / 2, (this->y() + other.y()) / 2,
                     (this->z() + other.z()) / 2);
}

Cartesian Cartesian::translated(const Cartesian& d) const
{
    return Cartesian(this->x() + d.x(), this->y() + d.y(), this->z() + d.z());
}

Cartesian Cartesian::rotated(float heading, const Cartesian& origin) const
{
    float headingR = qDegreesToRadians(-heading);

    return Cartesian(((this->x() - origin.x()) * qCos(headingR)) -
                         ((this->y() - origin.y()) * qSin(headingR)) + origin.x(),
                     ((this->x() - origin.x()) * qSin(headingR)) +
                         ((this->y() - origin.y()) * qCos(headingR)) + origin.y(),
                     this->z);
}

Cartesian Cartesian::operator-() const
{
    return Cartesian(-this->x(), -this->y(), -this->z());
}

namespace md::domain
{
bool operator==(const Cartesian& first, const Cartesian& second)
{
    return qFuzzyCompare(first.x, second.x) && qFuzzyCompare(first.y, second.y) &&
           qFuzzyCompare(first.z, second.z);
}
} // namespace md::domain
