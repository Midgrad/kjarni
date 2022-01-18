#ifndef I_COMMUNICATION_PROTOCOL_H
#define I_COMMUNICATION_PROTOCOL_H

#include "QByteArray"
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
    virtual void receiveData(const QByteArray& data) = 0;

signals:
    void finished();
    void sendData(QByteArray data);
};
} // namespace md::domain

#endif //I_COMMUNICATION_PROTOCOL_H
