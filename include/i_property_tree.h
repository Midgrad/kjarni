#ifndef I_PROPERTY_TREE_H
#define I_PROPERTY_TREE_H

#include "i_service.h"
#include "utils.h"

namespace kjarni::domain
{
class IPropertyTree : public IService
{
public:
    using PropertyCallback = std::function<void(const QJsonValue&)>;

    virtual QStringList rootNodes() const = 0;
    virtual QJsonObject property(const QString& path) const = 0;

    virtual void setProperties(const QString& path, const QJsonObject& properties) = 0;
    virtual void appendProperties(const QString& path, const QJsonObject& properties) = 0;
};
} // namespace kjarni::domain

#endif // I_PROPERTY_TREE_H
