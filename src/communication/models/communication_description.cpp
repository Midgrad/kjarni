#include "communication_description.h"

using namespace md::domain;

md::domain::CommunicationDescription::CommunicationDescription(
    const LinkSpecification& specification, const ProtocolSpecification& protocolDescription,
    const QString& name, QObject* parent) :
    domain::ICommunication(name, parent),
    m_protocolSpecification(),
    m_linkSpecification()
{
}

QString CommunicationDescription::type() const
{
    return m_linkSpecification.type();
}

bool CommunicationDescription::isConnected() const
{
    return m_connected;
}

int CommunicationDescription::bytesReceived() const
{
    return m_bytesReceived;
}

int CommunicationDescription::bytesSent() const
{
    return m_bytesSent;
}

void CommunicationDescription::setConnected(bool connected)
{
    if (m_connected)
        return;

    m_connected = connected;
    emit connectedChanged(m_connected);
}

void CommunicationDescription::setBytesReceived(int bytesReceived)
{
    if (m_bytesReceived == bytesReceived)
        return;
    
    m_bytesReceived = bytesReceived;
    emit bytesReceivedChanged(m_bytesReceived);
}

void CommunicationDescription::setBytesSent(int bytesSent)
{
    if (m_bytesSent == bytesSent)
        return;

    m_bytesSent = bytesSent;
    emit bytesSentChanged(m_bytesSent);
}