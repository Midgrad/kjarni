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
    return first.latitude == second.latitude && first.longitude == second.longitude &&
           first.altitude == second.altitude && first.datum == second.datum;
}
} // namespace md::domain
