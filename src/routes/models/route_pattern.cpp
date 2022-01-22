#include "route_pattern.h"

using namespace md::domain;

RoutePattern::RoutePattern(const RoutePatternType* type, QObject* parent) :
    PlainParametrised(type->name, utils::generateId(), type->defaultParameters()),
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
