#include "command.h"

using namespace md::domain;

Command::Command(QObject* parent) : QObject(parent)
{
}

void Command::subscribe(std::function<void(const QVariant&, const QVariantList&)> func,
                        QObject* owner)
{
    connect(this, &Command::exec, owner, func);
}
