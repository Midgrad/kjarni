#ifndef I_COMMUNICATION_H
#define I_COMMUNICATION_H

#include "named_mixin.hpp"

namespace md::domain
{
class ICommunication : public NamedMixin<Entity>
{
    Q_OBJECT

public:
    ICommunication(const QString& name, QObject* parent) :
        NamedMixin<Entity>(name, utils::generateId(), parent)
    {
    }
    virtual ~ICommunication() = default;

    virtual QString type() const = 0;
    virtual bool isConnected() const = 0;
    virtual int bytesReceived() const = 0;
    virtual int bytesSent() const = 0;

public slots:
    virtual void setConnected(bool) = 0;

signals:
    void connectedChanged(bool connected);
    void bytesReceivedChanged(int bytesReceived);
    void bytesSentChanged(int bytesSent);
};
} // namespace md::domain

#endif //I_COMMUNICATION_H
