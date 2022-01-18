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

bool CartesianPath::isEmpty() const
{
    return this->positions().isEmpty();
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

QVector<CartesianLine> CartesianPath::lines(bool closed) const
{
    if (this->positions().length() < 2)
        return {};

    if (this->positions().length() == 2)
        return { CartesianLine(this->positions().at(0), this->positions().at(1)) };

    QVector<CartesianLine> lines;
    for (int i = 0; i < this->positions().length() - 1; ++i)
    {
        lines.append(CartesianLine(this->positions().at(i), this->positions().at(i + 1)));
    }

    if (closed)
    {
        lines.append(CartesianLine(this->positions().at(this->positions().length() - 1),
                                   this->positions().at(0)));
    }

    return lines;
}

CartesianRect CartesianPath::boundingRect() const
{
    if (this->isEmpty())
        return CartesianRect();

    Cartesian first = this->positions().first();
    double minX = first.x;
    double maxX = first.x;
    double minY = first.y;
    double maxY = first.y;
    float minZ = first.z;
    float maxZ = first.z;
    for (const Cartesian& pos : this->positions())
    {
        minX = qMin(minX, pos.x());
        maxX = qMax(maxX, pos.x());
        minY = qMin(minY, pos.y());
        maxY = qMax(maxY, pos.y());
        minZ = qMin(minZ, pos.z());
        maxZ = qMax(maxZ, pos.z());
    }
    return CartesianRect(Cartesian(minX, minY, minZ), Cartesian(maxX, maxY, maxZ));
}

QVector<Cartesian> CartesianPath::intersections2D(const CartesianLine& line, bool closed) const
{
    QVector<Cartesian> intersections;
    for (const CartesianLine& segment : this->lines(closed))
    {
        Cartesian intersection = segment.intersection2d(line);
        if (intersection.isValid())
            intersections.append(intersection);
    }

    return intersections;
}

QVector<Cartesian> CartesianPath::rotated(float heading, const Cartesian& origin) const
{
    QVector<Cartesian> rotatedPath;
    for (const Cartesian& pos : this->positions())
    {
        rotatedPath.append(pos.rotated(heading, origin));
    }
    return rotatedPath;
}

QVector<Cartesian> CartesianPath::sortedByDistance(const Cartesian& origin) const
{
    QVector<Cartesian> sortedPath = this->positions();

    std::sort(sortedPath.begin(), sortedPath.end(),
              [origin](const Cartesian& first, const Cartesian& second) {
                  return first.distanceTo(origin) > second.distanceTo(origin);
              });

    return sortedPath;
}
