#include "parametrised.h"

using namespace md::domain;

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

Parameter::Parameter(const QString& id, const QString& name, bool defaultValue) :
    Parameter(id, name, Bool, defaultValue, QVariant(), QVariant(), QVariant())
{
}

QVariantMap Parameter::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, id);
    map.insert(props::name, name);
    map.insert(props::defaultValue, defaultValue);
    map.insert(props::minValue, minValue);
    map.insert(props::maxValue, maxValue);
    map.insert(props::step, step);
    map.insert(props::type, QVariant::fromValue(type).toString());
    return map;
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

Parametrised::Parametrised(const QVariant& id, const QString& name, const QVariantMap& parameters,
                           QObject* parent) :
    Named(name, id, parent),
    m_parameters(parameters)
{
}

Parametrised::Parametrised(const QVariantMap& map, QObject* parent) :
    Parametrised(map.value(props::id, utils::generateId()), map.value(props::name).toString(),
                 map.value(props::params).toMap(), parent)
{
}

const QVariantMap& Parametrised::parameters() const
{
    return m_parameters;
}

QVariant Parametrised::parameter(const QString& key) const
{
    return m_parameters.value(key);
}

bool Parametrised::hasParameter(const QString& key) const
{
    return m_parameters.contains(key);
}

QVariantMap Parametrised::toVariantMap() const
{
    QVariantMap map = Named::toVariantMap();
    map.insert(props::params, m_parameters);
    return map;
}

void Parametrised::fromVariantMap(const QVariantMap& map)
{
    m_parameters = map.value(props::params, m_parameters).toMap();
    Named::fromVariantMap(map); // emit changed here
}

void Parametrised::setParameters(const QVariantMap& parameters)
{
    if (m_parameters == parameters)
        return;

    m_parameters = parameters;
    emit changed();
}

void Parametrised::setParameter(const QString& key, const QVariant& value)
{
    if (m_parameters.value(key) == value)
        return;

    m_parameters[key] = value;
    emit changed();
}

void Parametrised::setParameter(const Parameter* parameter, const QVariant& value)
{
    this->setParameter(parameter->id, value);
}

void Parametrised::removeParameters(const QStringList& keys)
{
    bool changedFlag = false;

    for (const QString& key : keys)
    {
        if (m_parameters.remove(key))
        {
            changedFlag = true;
        }
    }

    if (changedFlag)
        emit changed();
}

void Parametrised::removeParameter(const QString& key)
{
    if (m_parameters.remove(key))
    {
        emit changed();
    }
}
