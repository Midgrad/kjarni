#include "communication.h"

#include "link_transceiver.h"
#include "link_transceiver_threaded.h"

namespace
{
md::data_source::ILinkTransceiver* createLinkTranceiver(
    const md::domain::LinkSpecification& specification, QObject* parent)
{
    // TODO: check thread safety for a factory
    auto linkT = new md::data_source::LinkTransceiver(specification, nullptr);
    return new md::data_source::LinkTransceiverThreaded(linkT, parent);
}
} // namespace

using namespace md::data_source;

Communication::Communication(const domain::LinkSpecification& linkSpecification,
                             const domain::ProtocolSpecification& protocolDescription,
                             ICommunicationProtocol* protocol, const QString& name,
                             QObject* parent) :
    domain::ICommunication(name, parent),
    m_linkSpecification(linkSpecification),
    m_protocolDescription(protocolDescription),
    m_protocol(protocol),
    m_connected(false),
    m_bytesReceived(0),
    m_bytesSent(0)
{
    m_linkTransceiver = ::createLinkTranceiver(linkSpecification, this);

    QObject::connect(m_linkTransceiver, &ILinkTransceiver::receivedData, m_protocol,
                     &ICommunicationProtocol::receiveData);
    QObject::connect(m_protocol, &ICommunicationProtocol::sendData, m_linkTransceiver,
                     &ILinkTransceiver::send);
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

void Communication::setConnected(bool connected)
{
    if (m_connected)
        return;

    m_connected = connected;
    emit connectedChanged(m_connected);
}
