#include "vehicles_features.h"

using namespace md::domain;

VehiclesFeatures::VehiclesFeatures(QObject* parent) : IVehiclesFeatures(parent)
{
}

QVariant VehiclesFeatures::feature(const QString& id, const QString& type) const
{
    return m_features.value(id).value(type);
}

QVariantMap VehiclesFeatures::features(const QString& id) const
{
    return m_features.value(id);
}

void VehiclesFeatures::addFeature(const QString& id, const QString& type, const QVariant& feature)
{
    if (m_features.value(id).value(type) == feature)
        return;

    m_features[id][type] = feature;
    emit featureAdded(id, type, feature);
}

void VehiclesFeatures::removeFeature(const QString& id, const QString& type)
{
    if (!m_features.value(id).contains(type))
        return;

    m_features[id].remove(type);
    if (m_features.value(id).isEmpty())
        m_features.remove(id);
    emit featureRemoved(id, type);
}

void VehiclesFeatures::removeFeatures(const QString& id)
{
    for (const QString& type : m_features.value(id).keys())
    {
        this->removeFeature(id, type);
    }
}
