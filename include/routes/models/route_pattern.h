#ifndef ROUTE_PATTERN_H
#define ROUTE_PATTERN_H

#include "geodetic_path.h"
#include "route_pattern_type.h"

namespace md::domain
{
class RoutePattern : public Parametrised
{
    Q_OBJECT

public:
    explicit RoutePattern(const RoutePatternType* type, QObject* parent = nullptr);

    utils::ConstProperty<const RoutePatternType*> type;

    const GeodeticPath& area() const;
    const GeodeticPath& path() const;

    void setArea(const GeodeticPath& area);

public slots:
    virtual void calculate() = 0;

signals:
    void areaPositionsChanged();
    void pathPositionsChanged();

protected:
    GeodeticPath m_area;
    GeodeticPath m_path;
};

class IRoutePatternFactory
{
public:
    virtual ~IRoutePatternFactory() = default;

    virtual RoutePattern* create(const QString& routePatternId) = 0;
};
} // namespace md::domain

#endif // ROUTE_PATTERN_H
