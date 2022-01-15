#include "route_pattern.h"

using namespace md::domain;

RoutePattern::RoutePattern(const RoutePatternType* type, QObject* parent) :
    Parametrised(utils::generateId(), type->name, type->defaultParameters()),
    type(type)
{
    connect(this, &RoutePattern::changed, this, &RoutePattern::calculate);
}

const QVector<Geodetic>& RoutePattern::areaPositions() const
{
    return m_areaPositions;
}

const QVector<Geodetic>& RoutePattern::pathPositions() const
{
    return m_pathPositions;
}

void RoutePattern::setAreaPositions(const QVector<Geodetic>& areaPositions)
{
    m_areaPositions = areaPositions;
    emit areaPositionsChanged();

    this->calculate();
}
