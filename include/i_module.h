#ifndef I_MODULE_H
#define I_MODULE_H

#include <QObject>

#include "utils.h"

namespace kjarni::app
{
class IModule
{
public:
    IModule() = default;
    virtual ~IModule() = default;

    virtual void init()
    {
    }

    virtual void done()
    {
    }

    virtual void visit(QJsonObject& features)
    {
        Q_UNUSED(features);
    };
};
} // namespace kjarni::app

Q_DECLARE_INTERFACE(kjarni::app::IModule, "Midgrad.IModule")

#endif // I_MODULE_H
