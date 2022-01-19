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

void CommunicationDescription::connect()
{
    if (m_connected)
        return;

    m_connected = true;
    emit connectedChanged(m_connected);
}

void CommunicationDescription::disconnect()
{
    if (!m_connected)
        return;

    m_connected = false;
    emit connectedChanged(m_connected);
}