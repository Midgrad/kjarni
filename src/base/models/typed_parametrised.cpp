#include "typed_parametrised.h"

#include <QDebug>
#include <QMetaEnum>

using namespace md::domain;

namespace
{
ParameterType::Type typeEnumFromString(const QString& str)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<ParameterType::Type>();
    return static_cast<ParameterType::Type>(metaEnum.keyToValue(str.toUtf8().constData()));
}
} // namespace

ParameterType::ParameterType(const QString& id, const QString& name, Type type,
                             const QVariant& defaultValue, const QVariant& minValue,
                             const QVariant& maxValue, const QVariant& step,
                             const QVariantList& options) :
    id(id),
    name(name),
    type(type),
    defaultValue(defaultValue),
    minValue(minValue),
    maxValue(maxValue),
    step(step),
    options(options)
{
}

ParameterType::ParameterType(const QString& id, const QString& name, bool defaultValue) :
    ParameterType(id, name, Bool, defaultValue, QVariant(), QVariant(), QVariant())
{
}

ParameterType::ParameterType(const QString& id, const QString& name, const QString& defaultValue) :
    ParameterType(id, name, Text, defaultValue, QVariant(), QVariant(), QVariant())
{
}

ParameterType::ParameterType(const QString& id, const QString& name, const QVariantList& options,
                             const QVariant& defaultValue) :
    ParameterType(id, name, Combo, defaultValue, QVariant(), QVariant(), QVariant(), options)
{
}

ParameterType::ParameterType(const QVariantMap& map) :
    ParameterType(map.value(props::id).toString(), map.value(props::name).toString(),
                  ::typeEnumFromString(map.value(props::type).toString()),
                  map.value(props::defaultValue), map.value(props::minValue),
                  map.value(props::maxValue), map.value(props::step),
                  utils::variantListFromJoin(map.value(props::options).toString()))
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
    map.insert(props::options, utils::joinFromVariantList(options));
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

TypedParameter::TypedParameter(const ParameterType* type, const QVariant& value, QObject* parent) :
    QObject(parent),
    id(type->id),
    m_type(type),
    m_value(value)
{
}

TypedParameter::TypedParameter(const ParameterType* parameterType, QObject* parent) :
    TypedParameter(parameterType, parameterType->defaultValue, parent)
{
}

const ParameterType* TypedParameter::type() const
{
    return m_type;
}

QVariant TypedParameter::value() const
{
    return m_value;
}

void TypedParameter::setValue(const QVariant& value)
{
    if (m_value == value)
        return;

    m_value = m_type->guard(value);
    emit changed(value);
}

void TypedParameter::reset()
{
    this->setValue(m_type->defaultValue);
}
