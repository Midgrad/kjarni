#ifndef CARTESIAN_PATH_H
#define CARTESIAN_PATH_H

#include "cartesian_line.h"
#include "cartesian_rect.h"

#include <QVector>

namespace md::domain
{
class CartesianPath
{
public:
    CartesianPath(const QVector<Cartesian>& positions = {});
    CartesianPath(const QVariantList& list);

    utils::ConstProperty<QVector<Cartesian>> positions;

    bool isEmpty() const;
    QVariantList toVariantList() const;
    double distance() const;
    QVector<CartesianLine> lines(bool closed) const;
    CartesianRect boundingRect() const;
    Cartesian center() const;
    QVector<Cartesian> intersections2D(const CartesianLine& line, bool closed) const;
    QVector<Cartesian> rotated(float heading, const Cartesian& origin) const;
    QVector<Cartesian> sortedByDistance(const Cartesian& origin) const;
};

} // namespace md::domain

#endif // CARTESIAN_PATH_H
