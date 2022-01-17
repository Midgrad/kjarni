#include "geodetic_path.h"

using namespace md::domain;

namespace
{
QVector<Geodetic> variantListToPositions(const QVariantList& list)
{
    QVector<Geodetic> positions;
    for (const QVariant& variant : list)
    {
        positions.append(Geodetic(variant.toMap()));
    }
    return positions;
}
} // namespace

GeodeticPath::GeodeticPath(const QVector<Geodetic>& positions) : positions(positions)
{
}

GeodeticPath::GeodeticPath(const QVariantList& list) : GeodeticPath(::variantListToPositions(list))
{
}

bool GeodeticPath::isEmpty() const
{
    return this->positions().isEmpty();
}

QVariantList GeodeticPath::toVariantList() const
{
    QVariantList list;
    for (const Geodetic& position : this->positions())
    {
        list.append(position.toVariantMap());
    }
    return list;
}

double GeodeticPath::distance() const
{
    double dist = 0;
    for (int i = 0; i < this->positions().length() - 1; ++i)
    {
        dist += this->positions().at(i).distanceTo(this->positions().at(i + 1));
    }
    return dist;
}

CartesianPath GeodeticPath::nedPath(const Geodetic& origin) const
{
    QVector<Cartesian> cartesians;
    for (const Geodetic& position : this->positions())
    {
        cartesians.append(position.nedPoint(origin));
    }
    return cartesians;
}

GeodeticRect GeodeticPath::boundingRect() const
{
    if (this->isEmpty())
        return GeodeticRect();

    Geodetic first = this->positions().first();
    double minLat = first.latitude;
    double maxLat = first.latitude;
    double minLon = first.longitude;
    double maxLon = first.longitude;
    float minAlt = first.altitude;
    float maxAlt = first.altitude;
    for (const Geodetic& pos : this->positions())
    {
        minLat = qMin(minLat, pos.latitude());
        maxLat = qMax(maxLat, pos.latitude());
        minLon = qMin(minLon, pos.longitude());
        maxLon = qMax(maxLon, pos.longitude());
        minAlt = qMin(minAlt, pos.altitude());
        maxAlt = qMax(maxAlt, pos.altitude());
    }
    return GeodeticRect(Geodetic(minLat, minLon, minAlt), Geodetic(maxLat, maxLon, maxAlt));
}
