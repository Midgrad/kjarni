#ifndef I_COMMUNICATION_PROTOCOL_H
#define I_COMMUNICATION_PROTOCOL_H

#include "export.h"

#include <QByteArray>
#include <QObject>

namespace md::data_source
{
class KJARNI_EXPORT ICommunicationProtocol : public QObject
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
} // namespace md::data_source

#endif //I_COMMUNICATION_PROTOCOL_H
