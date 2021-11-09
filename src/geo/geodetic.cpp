#include "geodetic.h"

#include <QtMath>

using namespace md::domain;

Geodetic::Geodetic(double latitude, double longitude, float altitude, const QString& datum) :
    m_latitude(latitude),
    m_longitude(longitude),
    m_altitude(altitude),
    m_datum(datum)
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

double Geodetic::latitude() const
{
    return m_latitude;
}

double Geodetic::longitude() const
{
    return m_longitude;
}

float Geodetic::altitude() const
{
    return m_altitude;
}

QString Geodetic::datum() const
{
    return m_datum;
}

QVariantMap Geodetic::toVariantMap() const
{
    return QVariantMap{ { geo::latitude, m_latitude },
                        { geo::longitude, m_longitude },
                        { geo::altitude, m_altitude },
                        { geo::datum, m_datum } };
}

bool Geodetic::isValidPosition() const
{
    return !std::isnan(m_latitude) && !std::isnan(m_longitude);
}

bool Geodetic::isValidAltitude() const
{
    return !std::isnan(m_altitude);
}

bool Geodetic::isValid() const
{
    return this->isValidPosition() && this->isValidAltitude();
}

Geodetic Geodetic::offsetted(double dLatitude, double dLongitude, float dAltitude) const
{
    return Geodetic(m_latitude + dLatitude, m_longitude + dLongitude, m_altitude + dAltitude,
                    m_datum);
}

Geodetic& Geodetic::operator=(const Geodetic& other)
{
    m_latitude = other.m_latitude;
    m_longitude = other.m_longitude;
    m_altitude = other.m_altitude;
    m_datum = other.m_datum;

    return *this;
}

namespace md::domain
{
bool operator==(const Geodetic& first, const Geodetic& second)
{
    return first.m_latitude == second.m_latitude && first.m_longitude == second.m_longitude &&
           first.m_altitude == second.m_altitude && first.m_datum == second.m_datum;
}
} // namespace md::domain
