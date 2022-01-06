#ifndef I_VEHICLES_FEATURES_H
#define I_VEHICLES_FEATURES_H

#include "i_service.h"
#include "vehicle_traits.h"

#include <QVariant>

namespace md::domain
{
class IVehiclesFeatures
    : public QObject
    , public IService
{
    Q_OBJECT

public:
    IVehiclesFeatures(QObject* parent) : QObject(parent), IService()
    {
    }

    virtual QVariant feature(const QString& id, const QString& type) const = 0;
    virtual QVariantMap features(const QString& id) const = 0;

public slots:
    virtual void addFeature(const QString& id, const QString& type, const QVariant& feature) = 0;
    virtual void removeFeature(const QString& id, const QString& type) = 0;
    virtual void removeFeatures(const QString& id) = 0;

signals:
    void featureAdded(QString id, QString type, QVariant feature);
    void featureRemoved(QString id, QString type);
};
} // namespace md::domain

#endif // I_VEHICLES_FEATURES_H
