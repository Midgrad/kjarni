#include "communication.h"

#include "link_transceiver.h"
#include "link_transceiver_threaded.h"

namespace
{
md::domain::ILinkTransceiver* createLinkTranceiver(
    const md::domain::LinkSpecification& specification, QObject* parent)
{
    // TODO: check thread safety for a factory
    auto linkT = new md::data_source::LinkTransceiver(specification, nullptr);
    return new md::data_source::LinkTransceiverThreaded(linkT, parent);
}
} // namespace

using namespace md::domain;

Communication::Communication(const LinkSpecification& linkSpecification,
                             const ProtocolDescription& protocolDescription, const QString& name,
                             QObject* parent) :
    Named(name, utils::generateId(), parent),
    m_linkSpecification(linkSpecification),
    m_protocolDescription(protocolDescription),
    m_connected(false),
    m_bytesReceived(0),
    m_bytesSent(0)
{
    m_linkTransceiver = ::createLinkTranceiver(linkSpecification, this);

    QObject::connect(m_linkTransceiver, &domain::ILinkTransceiver::receivedData,
                     m_protocolDescription.protocol(),
                     &domain::ICommunicationProtocol::receiveData);
    QObject::connect(m_protocolDescription.protocol(), &domain::ICommunicationProtocol::sendData,
                     m_linkTransceiver, &domain::ILinkTransceiver::send);
}

void Communication::start()
{
    m_linkTransceiver->start();
}

void Communication::stop()
{
    m_linkTransceiver->stop();
    //    emit finished();
}

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

void Communication::connect()
{
    if (m_connected)
        return;

    m_connected = true;
    emit connectedChanged(m_connected);
}

void Communication::disconnect()
{
    if (!m_connected)
        return;

    m_connected = false;
    emit connectedChanged(m_connected);
}
