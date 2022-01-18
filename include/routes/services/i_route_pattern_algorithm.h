#ifndef I_ROUTE_PATTERN_ALGORITHM_H
#define I_ROUTE_PATTERN_ALGORITHM_H

#include "cartesian_path.h"

namespace md::domain
{
class IRoutePatternAlgorithm
{
public:
    IRoutePatternAlgorithm(const QVector<Cartesian>& area, const QVariantMap& parameters) :
        area(area),
        parameters(parameters)
    {
    }
    virtual ~IRoutePatternAlgorithm() = default;

    utils::ConstProperty<CartesianPath> area;
    utils::ConstProperty<QVariantMap> parameters;

    const QVector<Cartesian>& path()
    {
        return m_path;
    };

    virtual void calculate() = 0;

protected:
    QVector<Cartesian> m_path;
};
} // namespace md::domain

#endif // I_ROUTE_PATTERN_ALGORITHM_H
