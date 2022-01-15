#ifndef ROUTE_PATTERN_H
#define ROUTE_PATTERN_H

#include "geodetic.h"
#include "route_pattern_type.h"

namespace md::domain
{
class RoutePattern : public Parametrised
{
    Q_OBJECT

public:
    explicit RoutePattern(const RoutePatternType* type, QObject* parent = nullptr);

    utils::ConstProperty<const RoutePatternType*> type;

    const QVector<domain::Geodetic>& areaPositions() const;
    const QVector<domain::Geodetic>& pathPositions() const;

    void setAreaPositions(const QVector<domain::Geodetic>& areaPositions);

public slots:
    virtual void calculate() = 0;

signals:
    void areaPositionsChanged();
    void pathPositionsChanged();

protected:
    QVector<domain::Geodetic> m_areaPositions;
    QVector<domain::Geodetic> m_pathPositions;
};

class IRoutePatternFactory
{
public:
    virtual ~IRoutePatternFactory() = default;

    virtual RoutePattern* create(const QString& routePatternId) = 0;
};
} // namespace md::domain

#endif // ROUTE_PATTERN_H
