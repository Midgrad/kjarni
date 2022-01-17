#include "cartesian_path.h"

using namespace md::domain;

namespace
{
QVector<Cartesian> variantListToPositions(const QVariantList& list)
{
    QVector<Cartesian> positions;
    for (const QVariant& variant : list)
    {
        positions.append(Cartesian(variant.toMap()));
    }
    return positions;
}
} // namespace

CartesianPath::CartesianPath(const QVector<Cartesian>& positions) : positions(positions)
{
}

CartesianPath::CartesianPath(const QVariantList& list) :
    CartesianPath(::variantListToPositions(list))
{
}

QVariantList CartesianPath::toVariantList() const
{
    QVariantList list;
    for (const Cartesian& position : this->positions())
    {
        list.append(position.toVariantMap());
    }
    return list;
}

double CartesianPath::distance() const
{
    double dist = 0;
    for (int i = 0; i < this->positions().length() - 1; ++i)
    {
        dist += this->positions().at(i).distanceTo(this->positions().at(i + 1));
    }
    return dist;
}
