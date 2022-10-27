#ifndef I_PROPERTY_TREE_H
#define I_PROPERTY_TREE_H

#include "export.h"

#include "i_service.h"

#include <QObject>
#include <QVariantMap>

namespace md::domain
{
class KJARNI_EXPORT IPropertyTree // TODO: ITelemetryService
    : public QObject
    , public IService
{
    Q_OBJECT

public:
    IPropertyTree(QObject* parent) : QObject(parent)
    {
    }

    virtual QStringList rootNodes() const = 0;
    virtual QVariantMap properties(const QString& path) const = 0;

    virtual void setProperties(const QString& path, const QVariantMap& properties) = 0;
    virtual void appendProperties(const QString& path, const QVariantMap& properties) = 0;
    virtual void removeProperties(const QString& path, const QStringList& properties) = 0;
    virtual void removeNode(const QString& path) = 0;

signals:
    void rootNodesChanged(QStringList nodes);
    void propertiesChanged(QString path, QVariantMap properties);
};
} // namespace md::domain

#endif // I_PROPERTY_TREE_H
