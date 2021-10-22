#ifndef VEHICLE_H
#define VEHICLE_H

#include "entity.h"

namespace md::domain
{
class Vehicle : public Entity
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

    Vehicle(Type type, const QString& name, QObject* parent = nullptr);
    Vehicle(const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap(bool recursive = true) const override;
    void fromVariantMap(const QVariantMap& map) override;

    Type type() const;

public slots:
    void setType(Type type);

signals:
    void typeChanged(Type type);

private:
    Type m_type;

    Q_ENUM(Type)
};
} // namespace md::domain

#endif // VEHICLE_H
