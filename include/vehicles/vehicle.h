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

    Vehicle(Type type, const QVariant& id, const QString& name, QObject* parent = nullptr);

    QString id() const;
    Type type() const;

public slots:
    void setType(Type type);

signals:
    void typeChanged(Type type);

private:
    Type m_type;
};
} // namespace md::domain

#endif // VEHICLE_H
