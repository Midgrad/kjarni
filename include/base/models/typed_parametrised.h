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

class TypedParameter : public QObject
{
    Q_OBJECT

public:
    TypedParameter(const ParameterType* type, const QVariant& value, QObject* parent = nullptr);
    explicit TypedParameter(const ParameterType* parameterType, QObject* parent = nullptr);

    utils::ConstProperty<QString> id;

    const ParameterType* type() const;
    QVariant value() const;
    void setValue(const QVariant& value);
    void reset();

signals:
    void changed(QVariant value);

private:
    const ParameterType* m_type;
    QVariant m_value;
};

template<class Base>
class TypedParametrisedMixin : public Base
{
public:
    template<typename... Args>
    TypedParametrisedMixin(const QVector<const ParameterType*>& types, const Args&... args) :
        Base(args...)
    {
        this->resetTypeParameters(types);
    }

    template<typename... Args>
    TypedParametrisedMixin(const QVector<const ParameterType*>& types, const QVariantMap& params,
                           const Args&... args) :
        Base(args...)
    {
        this->resetTypeParameters(types);
        this->setParameters(params);
    }

    TypedParameter* parameter(const QString& id) const
    {
        return m_parameters.value(id, nullptr);
    }

    QVector<TypedParameter*> parameters() const
    {
        return m_parameters.values().toVector();
    }

    void resetTypeParameters()
    {
        for (TypedParameter* parameter : m_parameters.values())
        {
            parameter->reset();
        }
    }

    QVariantMap parametersMap() const
    {
        QVariantMap parameters;
        for (TypedParameter* parameter : m_parameters.values())
        {
            parameters.insert(parameter->id(), parameter->value());
        }
        return parameters;
    }

    void setParameters(const QVariantMap& parameters)
    {
        for (TypedParameter* parameter : m_parameters.values())
        {
            parameter->setValue(parameters.value(parameter->id(), parameter->type()->defaultValue));
        }
    }

    QVariantMap toVariantMap() const override
    {
        QVariantMap map = Base::toVariantMap();
        map.insert(props::params, this->parametersMap());
        return map;
    }

    void fromVariantMap(const QVariantMap& map) override
    {
        this->setParameters(map.value(props::params).toMap());
        Base::fromVariantMap(map);
    }

    void resetTypeParameters(const QVector<const ParameterType*>& types)
    {
        QStringList parametersToDelete = m_parameters.keys();
        for (const ParameterType* type : types)
        {
            // Skip existing params
            if (m_parameters.contains(type->id))
            {
                parametersToDelete.removeOne(type->id);
                continue;
            }

            // Add new params
            auto parameter = new TypedParameter(type, type->defaultValue);
            parameter->moveToThread(this->thread());
            parameter->setParent(this);
            m_parameters.insert(type->id, parameter);
            QObject::connect(parameter, &TypedParameter::changed, this, &Base::changed);
        }

        // Remove unneeded parameters
        for (const QString& id : parametersToDelete)
        {
            m_parameters.take(id)->deleteLater();
        }
        emit Base::changed();
    }

private:
    QMap<QString, TypedParameter*> m_parameters;
};

} // namespace md::domain

#endif // PARAMETRISED_H
