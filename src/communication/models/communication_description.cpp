#include "communication_description.h"

using namespace md::domain;

md::domain::CommunicationDescription::CommunicationDescription(
    const LinkSpecification& specification, const ProtocolSpecification& protocolDescription,
    bool connected, const QString& name, QObject* parent) :
    domain::ICommunication(name, parent),
    m_linkSpecification(specification),
    m_protocolSpecification(protocolDescription),
    m_connected(connected),
    m_bytesReceived(0),
    m_bytesSent(0)
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
QVariantMap CommunicationDescription::toVariantMap() const
{
    QVariantMap map = Named::toVariantMap();

    map.insert(props::state, this->isConnected());
    map.insert(props::type, this->m_linkSpecification.type());
    map.insert(props::params, this->m_linkSpecification.parameters());
    map.insert(props::protocol, this->m_protocolSpecification.name());

    return map;
}
const LinkSpecification& CommunicationDescription::linkSpecification() const
{
    return m_linkSpecification;
}
const ProtocolSpecification& CommunicationDescription::protocolSpecification() const
{
    return m_protocolSpecification;
}
