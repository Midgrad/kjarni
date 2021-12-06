#include "geodetic.h"

#include <QtMath>

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
    return QVariantMap{ { geo::latitude, latitude.get() },
                        { geo::longitude, longitude.get() },
                        { geo::altitude, altitude.get() },
                        { geo::datum, datum.get() } };
}

bool Geodetic::isValidPosition() const
{
    return !std::isnan(latitude) && !std::isnan(longitude);
}

bool Geodetic::isValidAltitude() const
{
    return !std::isnan(altitude);
}

bool Geodetic::isValid() const
{
    return this->isValidPosition() && this->isValidAltitude();
}

Geodetic Geodetic::offsetted(double dLatitude, double dLongitude, float dAltitude) const
{
    return Geodetic(latitude + dLatitude, longitude + dLongitude, altitude + dAltitude, datum);
}

Geodetic& Geodetic::operator=(const Geodetic& other)
{
    latitude = other.latitude;
    longitude = other.longitude;
    altitude = other.altitude;
    datum = other.datum;

    return *this;
}

namespace md::domain
{
bool operator==(const Geodetic& first, const Geodetic& second)
{
    return first.latitude == second.latitude && first.longitude == second.longitude &&
           first.altitude == second.altitude && first.datum == second.datum;
}
} // namespace md::domain
