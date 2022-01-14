#include "communication.h"

using namespace md::domain;

Communication::Communication(LinkSpecification linkSpecification, ProtocolDescription protocolDescription, const QString& name,
                             QObject* parent) :
    Named(name, utils::generateId(), parent),
    m_linkSpecification(linkSpecification),
    m_protocolDescription(protocolDescription),
    m_connected(false),
    m_bytesReceived(0),
    m_bytesSent(0),
{

}

using namespace md::domain;

QString Communication::type() const
{
    return m_linkSpecification.type();
}

bool Communication::isConnected() const
{
    return m_connected;
}

int Communication::bytesReceived() const
{
    return m_bytesReceived;
}

int Communication::bytesSent() const
{
    return m_bytesSent;
}

void Communication::Connect()
{
    if (m_connected == true)
        return;

    m_connected = true;
    emit connectedChanged(m_connected);
}

void Communication::Disconnect()
{
    if (m_connected == false)
        return;

    m_connected = false;
    emit connectedChanged(m_connected);
}
