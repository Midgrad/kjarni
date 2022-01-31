#include "route_pattern.h"

using namespace md::domain;

RoutePattern::RoutePattern(const RoutePatternType* type, QObject* parent) :
    ParametrisedMixin<NamedMixin<Entity>>(type->defaultParameters(),
                                               std::bind(&Entity::changed, this), type->name,
                                               utils::generateId()),
    type(type)
{
    connect(this, &RoutePattern::changed, this, &RoutePattern::calculate);
}

const GeodeticPath& RoutePattern::area() const
{
    return m_area;
}

const GeodeticPath& RoutePattern::path() const
{
    return m_path;
}

void RoutePattern::setArea(const GeodeticPath& area)
{
    m_area = area;
    emit areaPositionsChanged();

    this->calculate();
}
