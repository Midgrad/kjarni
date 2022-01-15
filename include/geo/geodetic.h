#ifndef GEODETIC_H
#define GEODETIC_H

#include <QVariantMap>

#include "cartesian.h"
#include "geo_traits.h"

namespace md::domain
{
class Geodetic
{
public:
    Geodetic(double latitude, double longitude, float altitude,
             const QString& datum = geo::datums::wgs84);
    Geodetic(const QVariantMap& map);
    Geodetic();

    utils::ConstProperty<double> latitude;
    utils::ConstProperty<double> longitude;
    utils::ConstProperty<float> altitude;
    utils::ConstProperty<QString> datum;

    QVariantMap toVariantMap() const;
    bool isValidPosition() const;
    bool isValidAltitude() const;
    bool isValid() const;

    Geodetic offsetted(const Cartesian& nedPoint) const;
    Cartesian nedPoint(const Geodetic& origin) const;

    Geodetic& operator=(const Geodetic& other);
    friend bool operator==(const Geodetic& first, const Geodetic& second);
};

bool operator==(const Geodetic& first, const Geodetic& second);

// TODO: qDebug() operator
} // namespace md::domain

#endif // GEODETIC_H
