#ifndef CARTESIAN_PATH_H
#define CARTESIAN_PATH_H

#include "cartesian.h"

#include <QVector>

namespace md::domain
{
class CartesianPath
{
public:
    CartesianPath(const QVector<Cartesian>& positions);
    CartesianPath(const QVariantList& list);
    CartesianPath();

    utils::ConstProperty<QVector<Cartesian>> positions;

    QVariantList toVariantList() const;
    double distance() const;
};

} // namespace md::domain

#endif // CARTESIAN_PATH_H
