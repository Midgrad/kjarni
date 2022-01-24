#ifndef COMMUNICATION_DESCRIPTION_H
#define COMMUNICATION_DESCRIPTION_H

#include "i_communication.h"
#include "named.h"

#include "link_specification.h"
#include "protocol_specification.h"

namespace md::domain
{
class CommunicationDescription : public ICommunication
{
public:
    CommunicationDescription(const LinkSpecification& specification,
                             const ProtocolSpecification& protocolDescription,
                             bool connected = false, const QString& name = QString(),
                             QObject* parent = nullptr);

    QString type() const override;
    bool isConnected() const override;
    int bytesReceived() const override;
    int bytesSent() const override;

    const LinkSpecification& linkSpecification() const;
    const ProtocolSpecification& protocolSpecification() const;

    QVariantMap toVariantMap() const override;

public slots:
    void setConnected(bool connected) override;
    void setBytesReceived(int bytesReceived);
    void setBytesSent(int bytesSent);

private:
    LinkSpecification m_linkSpecification;
    ProtocolSpecification m_protocolSpecification;
    bool m_connected;
    int m_bytesReceived;
    int m_bytesSent;
};
} // namespace md::domain

#endif //COMMUNICATION_DESCRIPTION_H
