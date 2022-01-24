#include "communication_service.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

#include "communication.h"
#include "communication_description.h"
#include "communication_traits.h"
#include "json_source_file.h"
#include "link_transceiver_threaded.h"

namespace
{
constexpr char protocol[] = "protocol";
constexpr char localPort[] = "local_port";

} // namespace

using namespace md::app;

CommunicationService::CommunicationService(const QString& fileName) :
    m_protocols(),
    m_communications(),
    m_source(new data_source::JsonSourceFile(fileName)),
    m_json(m_source->read())
{
}

void CommunicationService::createCommunication(data_source::ICommunicationProtocol* protocol,
                                               domain::ProtocolSpecification protocolSpecification)
{
    //TODO: rewrite after sql implementation
    for (const auto& value : m_json.array())
    {
        QJsonObject communicationConfig = value.toObject();
        QString protocolName = (communicationConfig.value(::protocol).toString());

        if (protocolName == protocolSpecification.name())
        {
            QString name = (communicationConfig.value(domain::link_parameters::name).toString());
            QString type = (communicationConfig.value(domain::link_parameters::type).toString());
            int port = (communicationConfig.value(::localPort).toInt());

            domain::LinkSpecification linkSpecification({ { domain::link_parameters::port, port },
                                                          { domain::link_parameters::type, type } });

            auto communication = new data_source::Communication(linkSpecification,
                                                                protocolSpecification, protocol,
                                                                name, this);
            communication->start();

            auto communicationDescription =
                new domain::CommunicationDescription(linkSpecification, protocolSpecification,
                                                     false, name, this);

            m_communications.insert(communicationDescription, communication);

            QObject::connect(communication, &domain::ICommunication::bytesReceivedChanged,
                             communicationDescription,
                             &domain::CommunicationDescription::setBytesReceived);
            QObject::connect(communication, &domain::ICommunication::bytesSentChanged,
                             communicationDescription,
                             &domain::CommunicationDescription::setBytesSent);
            QObject::connect(communication, &domain::ICommunication::connectedChanged,
                             communicationDescription, &domain::ICommunication::setConnected);
        }
    }
}

void CommunicationService::registerProtocol(const QString& name,
                                            md::data_source::ICommunicationProtocol* protocol)
{
    domain::ProtocolSpecification protocolSpecification(name);

    //TODO: rewrite properly
    if (m_protocols.contains(name))
    {
        qCritical() << "Duplicate protocol found!";
        return;
    }

    m_protocols.insert(name, protocol);
    m_protocolSpecifications.append(protocolSpecification);

    this->createCommunication(protocol, protocolSpecification);
}
