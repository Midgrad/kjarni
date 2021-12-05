#ifndef VEHICLE_H
#define VEHICLE_H

#include "parametrised.h"

namespace md::domain
{
class Vehicle : public Parametrised
{
    Q_OBJECT

public:
    enum Type
    {
        Generic,
        Auto,
        FixedWing,
        FlyingWing,
        Copter,
        RotaryWing,
        Vtol,
        Airship,
    };

    Vehicle(Type type, const QString& name, const QVariant& id = utils::generateId(),
            const QVariantMap& parameters = QVariantMap(), QObject* parent = nullptr);
    Vehicle(const QVariantMap& map, QObject* parent = nullptr);

    utils::Property<Type> type;

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    Q_ENUM(Type)
};
} // namespace md::domain

#endif // VEHICLE_H
