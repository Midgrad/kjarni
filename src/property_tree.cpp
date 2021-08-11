#include "property_tree.h"

using namespace kjarni::domain;

QJsonValue PropertyTree::property(const QString& path)
{
    return m_properties.value(path);
}

void PropertyTree::setProperty(const QString& path, const QJsonValue& value)
{
    m_properties.insert(path, value);
}
