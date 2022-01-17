#ifndef I_COMMUNICATION_PROTOCOL_H
#define I_COMMUNICATION_PROTOCOL_H

#include "QObject"
namespace md::domain
{
class ICommunicationProtocol : public QObject
{
    Q_OBJECT

public:
    ICommunicationProtocol(QObject* parent) : QObject(parent)
    {
    }
    virtual ~ICommunicationProtocol() = default;

public slots:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void receiveData(const QByteArray& data);

signals:
    void finished();
    void sendData(QByteArray data);
};
} // namespace md::domain

#endif //I_COMMUNICATION_PROTOCOL_H
