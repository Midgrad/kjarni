#ifndef I_PROPERTY_TREE_H
#define I_PROPERTY_TREE_H

#include "i_service.h"
#include "utils.h"

#include <QObject>

namespace md::domain
{
class IPropertyTree
    : public QObject
    , public IService
{
    Q_OBJECT

public:
    IPropertyTree(QObject* parent) : QObject(parent)
    {
    }

    virtual QStringList rootNodes() const = 0;
    virtual QJsonObject properties(const QString& path) const = 0;

    virtual void setProperties(const QString& path, const QJsonObject& properties) = 0;
    virtual void appendProperties(const QString& path, const QJsonObject& properties) = 0;
    virtual void removeNode(const QString& path) = 0;

signals:
    void rootNodesChanged(QStringList nodes);
    void propertiesChanged(QString path, QJsonObject properties);
};
} // namespace md::domain

#endif // I_PROPERTY_TREE_H
