#ifndef VEHICLES_FEATURES_H
#define VEHICLES_FEATURES_H

#include "export.h"

#include "i_vehicles_features.h"

#include <QMap>

namespace md::domain
{
class KJARNI_EXPORT VehiclesFeatures : public IVehiclesFeatures
{
    Q_OBJECT

public:
    explicit VehiclesFeatures(QObject* parent = nullptr);

    QVariant feature(const QString& id, const QString& type) const override;
    QVariantMap features(const QString& id) const override;

public slots:
    void addFeature(const QString& id, const QString& type, const QVariant& feature) override;
    void removeFeature(const QString& id, const QString& type) override;
    void removeFeatures(const QString& id) override;

private:
    QMap<QString, QVariantMap> m_features;
};
} // namespace md::domain

#endif // VEHICLES_FEATURES_H
