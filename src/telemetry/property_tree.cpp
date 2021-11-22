#include "property_tree.h"

using namespace md::domain;

PropertyTree::PropertyTree(QObject* parent) : IPropertyTree(parent)
{
}

QStringList PropertyTree::rootNodes() const
{
    return m_properties.keys();
}

QVariantMap PropertyTree::properties(const QString& path) const
{
    return m_properties.value(path);
}

void PropertyTree::setProperties(const QString& path, const QVariantMap& properties)
{
    bool contains = m_properties.contains(path);

    m_properties[path] = properties;
    emit propertiesChanged(path, properties);

    if (!contains)
        emit rootNodesChanged(m_properties.keys());
}

void PropertyTree::appendProperties(const QString& path, const QVariantMap& properties)
{
    // Merge with old params
    QVariantMap merged = m_properties.value(path);
    for (auto it = properties.constBegin(); it != properties.constEnd(); ++it)
    {
        merged[it.key()] = it.value();
    }

    this->setProperties(path, merged);
}

void PropertyTree::removeProperties(const QString& path, const QStringList& properties)
{
    if (!m_properties.contains(path))
        return;

    bool changed = false;
    for (const QString& key : properties)
    {
        if (m_properties[path].remove(key))
            changed = true;
    }

    if (changed)
        emit propertiesChanged(path, m_properties[path]);
}

void PropertyTree::removeNode(const QString& path)
{
    bool contains = m_properties.contains(path);
    if (!contains)
        return;

    m_properties.remove(path);
    emit rootNodesChanged(m_properties.keys());
    emit propertiesChanged(path, QVariantMap());
}
