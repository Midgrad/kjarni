#ifndef COMMAND_H
#define COMMAND_H

#include <functional>

#include <QObject>

namespace md::domain
{
class Command : public QObject
{
    Q_OBJECT

public:
    Command(QObject* parent = nullptr);

    void subscribe(std::function<void(const QVariant&, const QVariantList&)> func,
                   QObject* owner = nullptr);

signals:
    void exec(const QVariant& target, const QVariantList& args);
};
} // namespace md::domain

#endif // COMMAND_H
