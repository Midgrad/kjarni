#include "waypoint_item.h"

#include <QDebug>

#include "route_traits.h"

using namespace md::domain;

WaypointItem::WaypointItem(const WaypointItemType* type, const QVariant& id, QObject* parent) :
    Parametrised(id, type->shortName, type->defaultParameters(), parent),
    m_type(type)
{
}

WaypointItem::WaypointItem(const WaypointItemType* type, const QVariantMap& map, QObject* parent) :
    Parametrised(map, parent),
    m_type(type)
{
    Q_ASSERT(type);
}

QVariantMap WaypointItem::toVariantMap() const
{
    QVariantMap map = Parametrised::toVariantMap();
    map.insert(props::type, m_type->id);
    return map;
}

void WaypointItem::fromVariantMap(const QVariantMap& map)
{
    Parametrised::fromVariantMap(map);
}

const WaypointItemType* WaypointItem::type() const
{
    return m_type;
}

void WaypointItem::setType(const WaypointItemType* type)
{
    Q_ASSERT(type);

    if (m_type == type)
        return;

    m_type = type;

    this->setName(type->shortName);
    this->syncParameters();
}

void WaypointItem::setAndCheckParameter(const QString& paramId, const QVariant& value)
{
    QVariant guarded = value;
    auto parameter = m_type->parameter(paramId);
    if (parameter)
    {
        guarded = parameter->guard(value);
    }
    this->setParameter(paramId, guarded);
}

void WaypointItem::resetParameter(const QString& paramId)
{
    auto parameter = m_type->parameter(paramId);
    if (!parameter)
        return;

    this->setParameter(paramId, parameter->defaultValue);
}

void WaypointItem::resetParameters()
{
    this->setParameters(m_type->defaultParameters());
}

void WaypointItem::syncParameters()
{
    QVariantMap parameters = this->parameters();

    // Add parameters defaulted by type
    for (const Parameter* parameter : m_type->parameters)
    {
        if (!parameters.contains(parameter->id))
            parameters.insert(parameter->id, parameter->defaultValue);
    }

    // Remove unneeded parameters
    for (const QString& paramId : parameters.keys())
    {
        if (!m_type->parameter(paramId))
            parameters.remove(paramId);
    }

    this->setParameters(parameters);
}
