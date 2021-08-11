#ifndef PROPERTY_TREE_H
#define PROPERTY_TREE_H

#include "i_property_tree.h"

namespace kjarni::domain
{
class PropertyTree : public IPropertyTree
{
public:
    QJsonValue property(const QString& path) override;
    void setProperty(const QString& path, const QJsonValue& value) override;

private:
    QJsonObject m_properties;
};
} // namespace kjarni::domain

#endif // PROPERTY_TREE_H
