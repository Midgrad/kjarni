#include "geodetic.h"

#include <QtMath>

namespace
{
constexpr double wgs84Radius = 6371008;
}

using namespace md::domain;

Geodetic::Geodetic(double latitude, double longitude, float altitude, const QString& datum) :
    latitude(latitude),
    longitude(longitude),
    altitude(altitude),
    datum(datum)
{
}

Geodetic::Geodetic(const QVariantMap& map) :
    Geodetic(map.value(geo::latitude, qQNaN()).toDouble(),
             map.value(geo::longitude, qQNaN()).toDouble(),
             map.value(geo::altitude, qQNaN()).toFloat(),
             map.value(geo::datum, geo::datums::wgs84).toString())
{
}

Geodetic::Geodetic() :
    Geodetic(std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(),
             std::numeric_limits<float>::quiet_NaN())

{
}

QVariantMap Geodetic::toVariantMap() const
{
    return QVariantMap{ { geo::latitude, this->latitude() },
                        { geo::longitude, this->longitude() },
                        { geo::altitude, this->altitude() },
                        { geo::datum, this->datum() } };
}

bool Geodetic::isValidPosition() const
{
    return !std::isnan(this->latitude()) && !std::isnan(this->longitude());
}

bool Geodetic::isValidAltitude() const
{
    return !std::isnan(this->altitude());
}

bool Geodetic::isValid() const
{
    return this->isValidPosition() && this->isValidAltitude();
}

Geodetic Geodetic::offsetted(double dLatitude, double dLongitude, float dAltitude) const
{
    return Geodetic(this->latitude() + dLatitude, this->longitude() + dLongitude,
                    this->altitude() + dAltitude, datum);
}

Geodetic Geodetic::offsetted(const Cartesian& nedPoint)
{
    if (!nedPoint.isValid() || !this->isValid())
        return Geodetic();

    double xRad = nedPoint.x / ::wgs84Radius;
    double yRad = nedPoint.y / ::wgs84Radius;
    double c = qSqrt(qPow(xRad, 2) + qPow(yRad, 2));
    double sinC = qSin(c);
    double cosC = qCos(c);

    double refLonR = qDegreesToRadians(this->longitude());
    double refLatR = qDegreesToRadians(this->latitude());

    double refSinLat = sin(refLatR);
    double refCosLat = cos(refLatR);

    double latR = 0;
    double lonR = 0;

    if (fabs(c) > std::numeric_limits<double>::epsilon())
    {
        latR = asin(cosC * refSinLat + (xRad * sinC * refCosLat) / c);
        lonR = (refLonR + atan2(yRad * sinC, c * refCosLat * cosC - xRad * refSinLat * sinC));
    }
    else
    {
        latR = refLatR;
        lonR = refLonR;
    }

    return Geodetic(qRadiansToDegrees(latR), qRadiansToDegrees(lonR),
                    -nedPoint.z + this->altitude());
}

Cartesian Geodetic::nedPoint(const Geodetic& origin)
{
    if (!origin.isValid() || !this->isValid())
        return Cartesian();

    if (origin == *this)
        return Cartesian(0, 0, 0);

    double latR = qDegreesToRadians(this->latitude);
    double lonR = qDegreesToRadians(this->longitude);

    double refLatR = qDegreesToRadians(origin.latitude);
    double refLonR = qDegreesToRadians(origin.longitude);

    double sinLatR = sin(latR);
    double cosLatR = cos(latR);
    double cosDLon = cos(lonR - refLonR);

    double refSinLatR = sin(refLatR);
    double refCosLatR = cos(refLatR);

    double c = acos(refSinLatR * sinLatR + refCosLatR * cosLatR * cosDLon);
    double k = (fabs(c) < std::numeric_limits<double>::epsilon()) ? 1.0 : (c / sin(c));

    return Cartesian(k * (refCosLatR * sinLatR - refSinLatR * cosLatR * cosDLon) * ::wgs84Radius,
                     k * cosLatR * sin(lonR - refLonR) * ::wgs84Radius,
                     -(this->altitude - origin.altitude));
}

Geodetic& Geodetic::operator=(const Geodetic& other)
{
    this->latitude = other.latitude;
    this->longitude = other.longitude;
    this->altitude = other.altitude;
    this->datum = other.datum;

    return *this;
}

namespace md::domain
{
bool operator==(const Geodetic& first, const Geodetic& second)
{
    return qFuzzyCompare(first.latitude, second.latitude) &&
           qFuzzyCompare(first.longitude, second.longitude) &&
           qFuzzyCompare(first.altitude, second.altitude) && first.datum == second.datum;
}
} // namespace md::domain
