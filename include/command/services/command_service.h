#ifndef COMMANDS_SERVICE_H
#define COMMANDS_SERVICE_H

#include "i_command_service.h"

#include <QMap>

namespace md::domain
{
// TODO: rename to CommandPool
class CommandsService : public ICommandsService
{
    Q_OBJECT

public:
    explicit CommandsService(QObject* parent = nullptr);

    Command* requestCommand(const QString& commandId) override;

private:
    QMap<QString, Command*> m_commands;
};
} // namespace md::domain

#endif // COMMANDS_SERVICE_H
