#ifndef PARAMETRISED_H
#define PARAMETRISED_H

#include "named.h"

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

class Parameter : public Entity
{
    Q_OBJECT

public:
    Parameter(const ParameterType* parameterType, const QVariant& value, QObject* parent = nullptr);
    explicit Parameter(const ParameterType* parameterType, QObject* parent = nullptr);

    const ParameterType* type() const;
    QVariant value() const;
    void setValue(const QVariant& value);

private:
    const ParameterType* m_parameterType;
    QVariant m_value;
};

//class TypedParametrised : public Named
//{
//    Q_OBJECT

//protected:
//    QMap<QString, Parameter*> m_parameters;
//};
} // namespace md::domain

#endif // PARAMETRISED_H
