#include "property_tree.h"

using namespace kjarni::domain;

PropertyTree::PropertyTree(QObject* parent) : IPropertyTree(parent)
{
}

QStringList PropertyTree::rootNodes() const
{
    return m_properties.keys();
}

QJsonObject PropertyTree::property(const QString& path) const
{
    return m_properties.value(path);
}

void PropertyTree::setProperties(const QString& path, const QJsonObject& property)
{
    bool contains = m_properties.contains(path);

    m_properties.insert(path, property);
    emit propertiesChanged(path, property);

    if (!contains)
        emit nodesChanged(m_properties.keys());
}

void PropertyTree::appendProperties(const QString& path, const QJsonObject& properties)
{
    bool contains = m_properties.contains(path);
    if (contains)
        emit propertiesChanged(path, utils::mergeJson(m_properties[path], properties));
    else
        this->setProperties(path, properties);
}

void PropertyTree::removeNode(const QString& path)
{
    bool contains = m_properties.contains(path);
    if (!contains)
        return;

    m_properties.remove(path);
    emit nodesChanged(m_properties.keys());
}
