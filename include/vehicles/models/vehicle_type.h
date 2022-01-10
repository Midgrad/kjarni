#ifndef VEHICLE_TYPE_H
#define VEHICLE_TYPE_H

#include <QObject>
#include <QVariantMap>

namespace md::domain
{
class VehicleType
{
    Q_GADGET

public:
    VehicleType(const QString& id, const QString& name, const QString& icon, const QString& model);

    QVariantMap toVariantMap() const;

    const QString id;
    const QString name;
    const QString icon;
    const QString model;
};
} // namespace md::domain

#endif // VEHICLE_TYPE_H
