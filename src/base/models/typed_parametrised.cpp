#include "typed_parametrised.h"

using namespace md::domain;

ParameterType::ParameterType(const QString& id, const QString& name, Type type,
                             const QVariant& defaultValue, const QVariant& minValue,
                             const QVariant& maxValue, const QVariant& step,
                             const QVariantList& variants) :
    id(id),
    name(name),
    type(type),
    defaultValue(defaultValue),
    minValue(minValue),
    maxValue(maxValue),
    step(step),
    variants(variants)
{
}

ParameterType::ParameterType(const QString& id, const QString& name, bool defaultValue) :
    ParameterType(id, name, Bool, defaultValue, QVariant(), QVariant(), QVariant())
{
}

ParameterType::ParameterType(const QString& id, const QString& name, const QVariantList& variants,
                             const QVariant& defaultValue) :
    ParameterType(id, name, Combo, defaultValue, QVariant(), QVariant(), QVariant(), variants)
{
}

QVariantMap ParameterType::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, id);
    map.insert(props::name, name);
    map.insert(props::defaultValue, defaultValue);
    map.insert(props::minValue, minValue);
    map.insert(props::maxValue, maxValue);
    map.insert(props::step, step);
    map.insert(props::variants, variants);
    map.insert(props::type, QVariant::fromValue(type).toString());
    return map;
}

QVariant ParameterType::guard(const QVariant& value) const
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

Parameter::Parameter(const ParameterType* parameterType, const QVariant& value, QObject* parent) :
    Entity(parameterType->id, parent),
    m_parameterType(parameterType),
    m_value(value)
{
}

Parameter::Parameter(const ParameterType* parameterType, QObject* parent) :
    Parameter(parameterType, parameterType->defaultValue, parent)

{
}

const ParameterType* Parameter::type() const
{
    return m_parameterType;
}

QVariant Parameter::value() const
{
    return m_value;
}

void Parameter::setValue(const QVariant& value)
{
    if (m_value == value)
        return;

    m_value = m_parameterType->guard(value);
    emit changed();
}
