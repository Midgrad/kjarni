#include "communication_service.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

#include "json_source_file.h"
#include "link_factory.h"
#include "link_transceiver.h"
#include "link_transceiver_threaded.h"

using namespace md::app;

namespace
{
constexpr char type[] = "type";
constexpr char name[] = "name";
constexpr char localPort[] = "local_port";

} // namespace

CommunicationService::CommunicationService(const QString& fileName) :
    m_source(new data_source::JsonSourceFile(fileName)),
    m_factory()
{
    m_links = createLinks();
}

md::data_source::LinkPtrMap CommunicationService::createLinks()
{
    QJsonDocument document = m_source->read();

    QMap<QString, md::data_source::LinkPtr> links;
    for (const QJsonValue& value : document.array())
    {
        QJsonObject linkConfig = value.toObject();

        QString type = (linkConfig.value(::type).toString());
        data_source::LinkPtr link;

        if (type == "udp")
            link = md::data_source::LinkPtr(
                m_factory.create(loodsman::LinkType::udp, linkConfig.value(::localPort).toInt()));
        else if (type == "tcp")
            link = md::data_source::LinkPtr(
                m_factory.create(loodsman::LinkType::tcp, linkConfig.value(::localPort).toInt()));
        else
            qWarning() << "Wrong link type in " << linkConfig.value(::name).toString();

        if (link)
            links[linkConfig.value(::name).toString()] = link;
    }

    return links;
}

void CommunicationService::createLinkTranceivers()
{
    for (const auto& link : m_links)
    {
        // TODO: check thread safety for factory
        auto linkT = new data_source::LinkTransceiver(link, nullptr);
        auto linkTT = new data_source::LinkTransceiverThreaded(linkT, this);
        m_linkTransceiverThreaded.append(linkTT);

        //        QObject::connect(linkT, &data_source::LinkTransceiver::receivedData, m_proto,
        //                         &domain::ICommunicationProtocol::receiveData);
        //        QObject::connect(this, &domain::ICommunicationProtocol::sendData, linkT,
        //                         &data_source::LinkTransceiver::send);
    }
}

md::data_source::LinkPtrMap CommunicationService::links()
{
    return m_links;
}

loodsman::LinkFactory* CommunicationService::factory()
{
    return &m_factory;
}

void CommunicationService::registerProtocol(const QString& name,
                                            md::domain::ICommunicationProtocol* const protocol)
{
    domain::ProtocolDescription protocolDescription(protocol, name, this);
    m_protocols.append(protocolDescription);
}