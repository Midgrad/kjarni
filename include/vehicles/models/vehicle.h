#ifndef VEHICLE_H
#define VEHICLE_H

#include "parametrised.h"

namespace md::domain
{
class Vehicle : public Parametrised
{
    Q_OBJECT

public:
    Vehicle(const QString& type, const QString& name, const QVariant& id = utils::generateId(),
            const QVariantMap& parameters = QVariantMap(), QObject* parent = nullptr);
    Vehicle(const QVariantMap& map, QObject* parent = nullptr);

    utils::Property<QString> type;
    utils::Property<bool> online;

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;
};
} // namespace md::domain

#endif // VEHICLE_H
