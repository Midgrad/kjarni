#ifndef PARAMETRISED_H
#define PARAMETRISED_H

#include "named_mixin.hpp"

#include <QVector>

namespace md::domain
{
class ParameterType
{
    Q_GADGET

public:
    enum Type
    {
        Bool,
        Int,
        Real,
        LatLon,
        Combo
    };

    ParameterType(const QString& id, const QString& name, Type type, const QVariant& defaultValue,
                  const QVariant& minValue, const QVariant& maxValue, const QVariant& step = 1,
                  const QVariantList& variants = {});
    ParameterType(const QString& id, const QString& name, bool defaultValue = false);
    ParameterType(const QString& id, const QString& name, const QVariantList& variants,
                  const QVariant& defaultValue);

    QVariantMap toVariantMap() const;
    QVariant guard(const QVariant& value) const;

    const QString id;
    const QString name;
    const Type type;
    const QVariant defaultValue;
    const QVariant minValue;
    const QVariant maxValue;
    const QVariant step;
    const QVariantList variants;

    Q_ENUM(Type)
};

class TypedParameter : public Entity
{
    Q_OBJECT

public:
    TypedParameter(const ParameterType* type, const QVariant& value, QObject* parent = nullptr);
    explicit TypedParameter(const ParameterType* parameterType, QObject* parent = nullptr);

    const ParameterType* type() const;
    QVariant value() const;
    void setValue(const QVariant& value);

    static fromTypeVector
private:
    const ParameterType* m_type;
    QVariant m_value;
};

template<class Base>
class TypedParametrisedMixin : public Base
{
public:
    template<typename... Args>
    TypedParametrisedMixin(const QVector<TypedParameter* const>& parameters, const Args&... args) :
        Base(args...),
        m_parameters(utils::vecToMap<TypedParameter* const>(parameters))
    {
    }

    template<typename... Args>
    TypedParametrisedMixin(const QVector<const ParameterType*>& parameterTypes,
                           const QVariantMap& parameters,
                           const Args&... args):
        TypedParametrisedMixin(TypedParameter::fromTypeVector(parameterTypes, parameters), args)
    {}

    TypedParameter* parameter(const QString& id) const
    {
        return m_parameters.value(id, nullptr);
    }

    QVariantMap toVariantMap() const override
    {
        QVariantMap map = Base::toVariantMap();
        QVariantMap parameters;
        for (TypedParameter* const parameter: m_parameters)
        {
            parameters.insert(parameter->id().toString(), parameter->value());
        }
        map.insert(props::params, parameters);
        return map;
    }

    void fromVariantMap(const QVariantMap& map) override
    {
        QVariantMap parameters = map.value(props::params).toMap();
        for (TypedParameter* const parameter: m_parameters)
        {
            parameter->setValue(parameters.value(parameter->id().toString(), parameter->type()->defaultValue));
        }
        Base::fromVariantMap(map);
    }

private:
    const QMap<QString, TypedParameter* const> m_parameters;
};

} // namespace md::domain

#endif // PARAMETRISED_H
