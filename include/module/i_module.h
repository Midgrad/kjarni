#ifndef I_MODULE_H
#define I_MODULE_H

#include <QObject>

#include "utils.h"

namespace md::app
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
} // namespace md::app

Q_DECLARE_INTERFACE(md::app::IModule, "Midgrad.IModule")

#endif // I_MODULE_H
