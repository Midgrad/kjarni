#include "command_service.h"

#include <QDebug>

using namespace md::domain;

CommandsService::CommandsService(QObject* parent) : ICommandsService(parent)
{
}

Command* CommandsService::requestCommand(const QString& commandId)
{
    Command* command = m_commands.value(commandId, nullptr);
    if (!command)
    {
        command = new Command();
        command->moveToThread(this->thread());
        command->setParent(this);
        m_commands.insert(commandId, command);
    }

    return command;
}
