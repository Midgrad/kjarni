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

GeodeticPath::GeodeticPath() : GeodeticPath(QVector<Geodetic>({}))
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
