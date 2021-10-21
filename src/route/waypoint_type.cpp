#include "waypoint_type.h"

#include "waypoint.h"

using namespace md::domain;

Parameter::Parameter(const QString& name, Type type, const QVariant& defaultValue,
                     const QVariant& minValue, const QVariant& maxValue, const QVariant& step) :
    name(name),
    type(type),
    defaultValue(defaultValue),
    minValue(minValue),
    maxValue(maxValue),
    step(step)
{
}

Parameter::Parameter(const Parameter& other) :
    Parameter(other.name, other.type, other.defaultValue, other.minValue, other.maxValue, other.step)
{
}

QVariant Parameter::guard(const QVariant& value) const
{
    QVariant result = value;
    if (!minValue.isNull())
        result = qMax(result, minValue);

    if (!maxValue.isNull())
        result = qMin(result, maxValue);

    return value;
}

WaypointType::WaypointType(const QString& name, const QVector<const Parameter*>& parameters) :
    name(name)
{
    for (const Parameter* parameter : parameters)
    {
        this->parameters[parameter->name] = parameter;
    }
}

const Parameter* WaypointType::parameter(const QString& name) const
{
    return this->parameters.value(name, nullptr);
}
