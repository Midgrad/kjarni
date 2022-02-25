#ifndef PARAMETER_H
#define PARAMETER_H

#include "entity.h"
#include "named_mixin.hpp"

namespace md::domain
{
class Parameter : public NamedMixin<Entity>
{
    Q_OBJECT

public:
    enum Type
    {
        Bool,
        Int,
        Real,
        LatLon,
        List
    };
    Q_ENUM(Type)

    Parameter(const QVariant& id, const QString& name, Type type, const QVariant& defaultValue,
              const QVariant& minValue, const QVariant& maxValue, const QVariant& step = 1,
              const QVariantList& variants = {});
    Parameter(const QVariant& id, const QString& name, bool defaultValue = false);
    Parameter(const QVariant& id, const QString& name, const QVariantList& variants,
              const QVariant& defaultValue);

    utils::ConstProperty<Type> type;
    utils::ConstProperty<QVariant> defaultValue;
    utils::ConstProperty<QVariant> minValue;
    utils::ConstProperty<QVariant> maxValue;
    utils::ConstProperty<QVariant> step;
    utils::ConstProperty<QVariantList> variants;

    utils::Property<QVariant> value;

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    QVariant guard(const QVariant& value) const;
    void setAndCheckValue(const QVariant& value);
};

using ParameterList = QList<Parameter*>;

} // namespace md::domain

#endif // PARAMETER_H
