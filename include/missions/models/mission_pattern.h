#ifndef MISSION_PATTERN_H
#define MISSION_PATTERN_H

#include "geodetic_path.h"
#include "mission_route_item.h"
#include "mission_pattern_type.h"
#include "parametrised_mixin.hpp"

namespace md::domain
{
class RoutePattern : public ParametrisedMixin<NamedMixin<Entity>>
{
    Q_OBJECT

public:
    explicit RoutePattern(const RoutePatternType* type, QObject* parent = nullptr);

    utils::ConstProperty<const RoutePatternType*> type;

    const GeodeticPath& area() const;
    const GeodeticPath& path() const;

    virtual bool isReady() const = 0;
    virtual QList<MissionRouteItem*> createItems() = 0;

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

#endif // MISSION_PATTERN_H
