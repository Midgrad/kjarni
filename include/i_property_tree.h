#ifndef I_PROPERTY_TREE_H
#define I_PROPERTY_TREE_H

#include <functional>

#include "utils.h"

namespace kjarni::domain
{
class IPropertyTree
{
public:
    using PropertyCallback = std::function<void(const QJsonValue&)>;

    IPropertyTree() = default;
    virtual ~IPropertyTree() = default;

    virtual void setData(const QString& path, const QJsonValue& value) = 0;
    virtual void subscribe(const QString& path, PropertyCallback callback) = 0;
    virtual void unsubscribe(const QString& path, PropertyCallback callback) = 0;
};
} // namespace kjarni::domain

#endif // I_PROPERTY_TREE_H
