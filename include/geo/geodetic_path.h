#ifndef GEODETIC_PATH_H
#define GEODETIC_PATH_H

#include "cartesian_path.h"
#include "geodetic_rect.h"

namespace md::domain
{
class GeodeticPath
{
public:
    GeodeticPath(const QVector<Geodetic>& positions = {});
    GeodeticPath(const QVariantList& list);

    utils::ConstProperty<QVector<Geodetic>> positions;

    bool isEmpty() const;
    QVariantList toVariantList() const;
    double distance() const;
    CartesianPath nedPath(const Geodetic& origin) const;
    GeodeticRect boundingRect() const;
};

} // namespace md::domain

#endif // GEODETIC_PATH_H
