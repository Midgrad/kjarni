#include "parameter.h"

#include "kjarni_traits.h"

using namespace md::domain;

Parameter::Parameter(const QVariant& id, const QString& name, Type type,
                     const QVariant& defaultValue, const QVariant& minValue,
                     const QVariant& maxValue, const QVariant& step, const QVariantList& variants) :
    NamedMixin<Entity>(name, id, nullptr),
    type(type),
    defaultValue(defaultValue),
    minValue(minValue),
    maxValue(maxValue),
    step(step),
    variants(variants),
    value(defaultValue, std::bind(&Entity::changed, this))
{
}

Parameter::Parameter(const QVariant& id, const QString& name, bool defaultValue) :
    Parameter(id, name, Bool, defaultValue, QVariant(), QVariant(), QVariant())
{
}

Parameter::Parameter(const QVariant& id, const QString& name, const QVariantList& variants,
                     const QVariant& defaultValue) :
    Parameter(id, name, List, defaultValue, QVariant(), QVariant(), QVariant(), variants)
{
}

QVariantMap Parameter::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, this->id());
    map.insert(props::name, this->name());
    map.insert(props::defaultValue, this->defaultValue());
    map.insert(props::minValue, this->minValue());
    map.insert(props::maxValue, this->maxValue());
    map.insert(props::step, this->step());
    map.insert(props::variants, this->variants());
    map.insert(props::type, QVariant::fromValue(this->type()).toString());
    map.insert(props::value, this->value());
    return map;
}

void Parameter::fromVariantMap(const QVariantMap& map)
{
    NamedMixin<Entity>::fromVariantMap(map);

    this->setAndCheckValue(map.value(props::value, this->value()));
}

QVariant Parameter::guard(const QVariant& value) const
{
    // Special case for null/NaN parameters
    if (value.isNull())
        return value;

    QVariant result = value;
    if (!this->minValue().isNull())
        result = qMax(result, this->minValue());

    if (!this->maxValue().isNull())
        result = qMin(result, this->maxValue());

    return result;
}

void Parameter::setAndCheckValue(const QVariant& value)
{
    this->value.set(this->guard(value));
}
