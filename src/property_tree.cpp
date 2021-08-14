#include "property_tree.h"

using namespace kjarni::domain;

QStringList PropertyTree::rootNodes() const
{
    return m_properties.keys();
}

QJsonObject PropertyTree::property(const QString& path) const
{
    return m_properties.value(path);
}

void PropertyTree::setProperty(const QString& path, const QJsonObject& property)
{
    m_properties.insert(path, property);
}
