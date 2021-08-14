#ifndef PROPERTY_TREE_H
#define PROPERTY_TREE_H

#include "i_property_tree.h"

#include <QMap>

namespace kjarni::domain
{
class PropertyTree : public IPropertyTree
{
public:
    QStringList rootNodes() const override;
    QJsonObject property(const QString& path) const override;

    void setProperty(const QString& path, const QJsonObject& property) override;

private:
    QMap<QString, QJsonObject> m_properties;
};
} // namespace kjarni::domain

#endif // PROPERTY_TREE_H
