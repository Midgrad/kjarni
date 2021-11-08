#include "waypoint_type.h"

#include "waypoint.h"

using namespace md::domain;

namespace
{
QMap<QString, const Parameter*> paramsToMap(const QVector<const Parameter*>& parameters)
{
    QMap<QString, const Parameter*> map;
    for (const Parameter* parameter : parameters)
    {
        map[parameter->id] = parameter;
    }
    return map;
}
} // namespace

Parameter::Parameter(const QString& id, const QString& name, Type type,
                     const QVariant& defaultValue, const QVariant& minValue,
                     const QVariant& maxValue, const QVariant& step) :
    id(id),
    name(name),
    type(type),
    defaultValue(defaultValue),
    minValue(minValue),
    maxValue(maxValue),
    step(step)
{
}

QVariant Parameter::guard(const QVariant& value) const
{
    // Special case for null/NaN parameters
    if (value.isNull())
        return value;

    QVariant result = value;
    if (!minValue.isNull())
        result = qMax(result, minValue);

    if (!maxValue.isNull())
        result = qMin(result, maxValue);

    return value;
}

WaypointType::WaypointType(const QString& id, const QString& name, const QString& shortName,
                           const QVector<const Parameter*>& parameters) :
    id(id),
    name(name),
    shortName(shortName),
    parameters(::paramsToMap(parameters))
{
}

const Parameter* WaypointType::parameter(const QString& id) const
{
    return this->parameters.value(id, nullptr);
}

QVariantMap WaypointType::defaultParameters() const
{
    QVariantMap map;
    for (const Parameter* parameter : parameters)
    {
        map.insert(parameter->id, parameter->defaultValue);
    }
    return map;
}
