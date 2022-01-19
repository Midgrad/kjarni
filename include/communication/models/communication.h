#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "named.h"

#include <QObject>
#include <QVariantMap>
#include <QVector>

#include "i_link_transceiver.h"
#include "link_specification.h"
#include "protocol_description.h"

namespace md::domain
{
class Communication : public Named
{
    Q_OBJECT

public:
    Communication(const LinkSpecification& linkSpecification,
                  const ProtocolDescription& protocolDescription, const QString& name = QString(),
                  QObject* parent = nullptr);

    QString type() const;

    bool isConnected() const;

    int bytesReceived() const;
    int bytesSent() const;

public slots:
    void connect();
    void disconnect();

    void start();
    void stop();

signals:
    void connectedChanged(bool connected);
    void bytesReceivedChanged(int bytesReceived);
    void bytesSentChanged(int bytesSent);

private:
    md::domain::ILinkTransceiver* createLinkTranceiver(
        const domain::LinkSpecification& specification);
    ILinkTransceiver* m_linkTransceiver;
    LinkSpecification m_linkSpecification;
    ProtocolDescription m_protocolDescription;
    bool m_connected;
    int m_bytesReceived;
    int m_bytesSent;
};
} // namespace md::domain

#endif //COMMUNICATION_H
