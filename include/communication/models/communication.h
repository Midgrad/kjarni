#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "i_link_transceiver.h"
#include "link_specification.h"
#include "link_traits.h"
#include "named.h"
#include "protocol_description.h"

#include <QObject>
#include <QVariantMap>
#include <QVector>

namespace md::domain
{
class Communication : public Named
{
    Q_OBJECT

public:
    explicit Communication(LinkSpecification linkSpecification,
                           ProtocolDescription protocolDescription, const QString& name = QString(),
                           QObject* parent = nullptr);

    QString type() const;

    bool isConnected() const;

    int bytesReceived() const;
    int bytesSent() const;

public slots:
    void Connect();
    void Disconnect();

    void start();
    void stop();

signals:
    void connectedChanged(bool connected);
    void bytesReceivedChanged(int bytesReceived);
    void bytesSentChanged(int bytesSent);

private:
    QVector<ILinkTransceiver*> m_linkTransceiverThreaded;
    LinkSpecification m_linkSpecification;
    ProtocolDescription m_protocolDescription;
    bool m_connected;
    int m_bytesReceived;
    int m_bytesSent;
};
} // namespace md::domain

#endif //COMMUNICATION_H
