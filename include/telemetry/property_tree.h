#ifndef PROPERTY_TREE_H
#define PROPERTY_TREE_H

#include "i_property_tree.h"

#include <QMap>

namespace md::domain
{
class PropertyTree : public IPropertyTree
{
    Q_OBJECT

public:
    PropertyTree(QObject* parent = nullptr);

    QStringList rootNodes() const override;
    QVariantMap properties(const QString& path) const override;

    void setProperties(const QString& path, const QVariantMap& properties) override;
    void appendProperties(const QString& path, const QVariantMap& properties) override;
    void removeProperties(const QString& path, const QStringList& properties) override;
    void removeNode(const QString& path) override;

private:
    QMap<QString, QVariantMap> m_properties;
};
} // namespace md::domain

#endif // PROPERTY_TREE_H
