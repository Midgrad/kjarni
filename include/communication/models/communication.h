#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "i_communication.h"
#include "named_mixin.hpp"

#include <QObject>
#include <QVariantMap>
#include <QVector>

#include "i_communication_protocol.h"
#include "i_link_transceiver.h"
#include "link_specification.h"
#include "protocol_specification.h"

namespace md::data_source
{
class Communication : public domain::ICommunication
{
    Q_OBJECT

public:
    Communication(const domain::LinkSpecification& linkSpecification,
                  const domain::ProtocolSpecification& protocolDescription,
                  ICommunicationProtocol* protocol, const QString& name = QString(),
                  QObject* parent = nullptr);

    QString type() const override;
    bool isConnected() const override;
    int bytesReceived() const override;
    int bytesSent() const override;

public slots:
    void setConnected(bool connected) override;
    void start();
    void stop();

private:
    ILinkTransceiver* m_linkTransceiver;
    domain::LinkSpecification m_linkSpecification;
    domain::ProtocolSpecification m_protocolDescription;
    const ICommunicationProtocol* m_protocol;
    bool m_connected;
    int m_bytesReceived;
    int m_bytesSent;
};
} // namespace md::data_source

#endif //COMMUNICATION_H
