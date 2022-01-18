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

Geodetic Geodetic::midPoint(const Geodetic& other) const
{
    return Geodetic((this->latitude() + other.latitude()) / 2,
                    (this->longitude() + other.longitude()) / 2,
                    (this->altitude() + other.altitude()) / 2);
}

Geodetic Geodetic::offsetted(const Cartesian& nedPoint) const
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

    double refSinLat = qSin(refLatR);
    double refCosLat = qCos(refLatR);

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
                    -nedPoint.z + this->altitude(), this->datum());
}

Cartesian Geodetic::nedPoint(const Geodetic& origin) const
{
    if (!origin.isValid() || !this->isValid())
        return Cartesian();

    if (origin == *this)
        return Cartesian(0, 0, 0);

    double latR = qDegreesToRadians(this->latitude);
    double lonR = qDegreesToRadians(this->longitude);

    double refLatR = qDegreesToRadians(origin.latitude);
    double refLonR = qDegreesToRadians(origin.longitude);

    double sinLatR = qSin(latR);
    double cosLatR = qCos(latR);
    double cosDLon = qCos(lonR - refLonR);

    double refSinLatR = qSin(refLatR);
    double refCosLatR = qCos(refLatR);

    double c = qAcos(refSinLatR * sinLatR + refCosLatR * cosLatR * cosDLon);
    double k = (fabs(c) < std::numeric_limits<double>::epsilon()) ? 1.0 : (c / sin(c));

    return Cartesian(k * (refCosLatR * sinLatR - refSinLatR * cosLatR * cosDLon) * ::wgs84Radius,
                     k * cosLatR * sin(lonR - refLonR) * ::wgs84Radius,
                     -(this->altitude - origin.altitude));
}

double Geodetic::distanceTo(const Geodetic& other) const
{
    double lat1R = qDegreesToRadians(this->latitude);
    double lon1R = qDegreesToRadians(this->longitude);

    double lat2R = qDegreesToRadians(other.latitude);
    double lon2R = qDegreesToRadians(other.longitude);

    double dLat = lat2R - lat1R;
    double dLon = lon2R - lon1R;

    double a = qSin(dLat / 2) * qSin(dLat / 2) +
               qCos(lat1R) * qCos(lat2R) * qSin(dLon / 2) * qSin(dLon / 2);
    double c = 2 * qAtan2(qSqrt(a), qSqrt(1 - a));

    return ::wgs84Radius * c;
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
