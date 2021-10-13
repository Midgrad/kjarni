#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

namespace md::domain
{
class Command : public QObject
{
    Q_OBJECT

public:
    Command(QObject* parent = nullptr);

    void subscribe(std::function<void(const QString& target, const QVariantList& args)> func,
                   QObject* owner = nullptr);

signals:
    void exec(const QString& target, const QVariantList& args);
};
} // namespace md::domain

#endif // COMMAND_H
