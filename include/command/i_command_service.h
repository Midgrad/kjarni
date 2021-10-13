#ifndef I_COMMANDS_SERVICE_H
#define I_COMMANDS_SERVICE_H

#include "command.h"
#include "i_service.h"

namespace md::domain
{
class ICommandsService
    : public QObject
    , public IService
{
    Q_OBJECT

public:
    ICommandsService(QObject* parent) : QObject(parent), IService()
    {
    }

    virtual Command* requestCommand(const QString& commandId) = 0;
};
} // namespace md::domain

#endif // I_COMMANDS_SERVICE_H
