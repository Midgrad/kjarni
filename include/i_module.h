#ifndef I_MODULE_H
#define I_MODULE_H

#include <QJsonObject>
#include <QObject>

namespace kjarni::app
{
class IModule
{
public:
    IModule() = default;
    virtual ~IModule() = default;

    virtual void init() = 0;
    virtual void done() = 0;

    virtual QJsonObject qmlEntries() const = 0;
};
} // namespace kjarni::app

Q_DECLARE_INTERFACE(kjarni::app::IModule, "Midgrad.IModule")

#endif // I_MODULE_H
