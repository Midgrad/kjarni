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

bool Parameter::isNull() const
{
    return name.isNull(); // This is enough
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

WaypointType::WaypointType(const QString& name, const QVector<Parameter>& parameters) :
    name(name),
    parameters(parameters)
{
}

Parameter WaypointType::parameter(const QString& name) const
{
    auto result = std::find_if(parameters.begin(), parameters.end(),
                               [name](const Parameter& parameter) {
                                   return parameter.name == name;
                               });
    if (result != std::end(parameters))
        return *result;

    return Parameter(QString());
}

void WaypointType::syncParameters(Waypoint* waypoint) const
{
    QStringList unneededParameters = waypoint->parameters().keys();
    for (const Parameter& parameter : this->parameters)
    {
        // If parameter exist - remove it from unneededParameters
        if (!unneededParameters.removeOne(parameter.name))
        {
            // Or add it with default value
            waypoint->setParameter(parameter.name, parameter.defaultValue);
        }
    }
    waypoint->removeParameters(unneededParameters);
}

void WaypointType::resetParameter(Waypoint* waypoint, const QString& key) const
{
    Parameter parameter = this->parameter(key);
    if (parameter.isNull())
        return;

    waypoint->setParameter(key, parameter.defaultValue);
}

namespace md::domain
{
bool operator==(const Parameter& left, const Parameter& right)
{
    return left.name == right.name;
}
} // namespace md::domain
