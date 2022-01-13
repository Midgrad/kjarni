#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "link_specification.h"
#include "link_traits.h"
#include "named.h"

#include <QObject>
#include <QVariantMap>

namespace md::domain
{
class Communication : public Named
{
    Q_OBJECT

public:
    explicit Communication(LinkSpecification linkSpecification, const QString& name = QString(),
                           QObject* parent = nullptr);

    QString type() const;

    bool isConnected() const;

    int bytesReceived() const;
    int bytesSent() const;

public slots:
    void Connect();
    void Disconnect();

signals:
    void connectedChanged(bool connected);
    void bytesReceivedChanged(int bytesReceived);
    void bytesSentChanged(int bytesSent);

private:
    LinkSpecification m_linkSpecification;
    bool m_connected;
    int m_bytesReceived;
    int m_bytesSent;
};
} // namespace md::domain

#endif //COMMUNICATION_H
