#ifndef VEHICLE_H
#define VEHICLE_H

#include "export.h"

#include "named_mixin.hpp"
#include "parametrised_mixin.hpp"
#include "vehicle_type.h"

namespace md::domain
{
class KJARNI_EXPORT Vehicle : public ParametrisedMixin<NamedMixin<Entity>>
{
    Q_OBJECT

public:
    Vehicle(const VehicleType* type, const QString& name, const QVariant& id = utils::generateId(),
            const QVariantMap& parameters = QVariantMap(), QObject* parent = nullptr);
    Vehicle(const VehicleType* type, const QVariantMap& map, QObject* parent = nullptr);

    utils::ConstProperty<const VehicleType*> type;
    utils::Property<bool> online;

    QVariantMap toVariantMap() const override;
};
} // namespace md::domain

#endif // VEHICLE_H
