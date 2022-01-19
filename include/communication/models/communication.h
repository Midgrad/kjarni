#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "named.h"

#include <QObject>
#include <QVariantMap>
#include <QVector>

#include "i_link_transceiver.h"
#include "link_specification.h"
#include "protocol_description.h"

namespace md::data_source
{
class Communication : public domain::Named
{
    Q_OBJECT

public:
    Communication(const domain::LinkSpecification& linkSpecification,
                  const domain::ProtocolDescription& protocolDescription,
                  const QString& name = QString(), QObject* parent = nullptr);

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
    ILinkTransceiver* m_linkTransceiver;
    domain::LinkSpecification m_linkSpecification;
    domain::ProtocolDescription m_protocolDescription;
    bool m_connected;
    int m_bytesReceived;
    int m_bytesSent;
};
} // namespace md::data_source

#endif //COMMUNICATION_H
