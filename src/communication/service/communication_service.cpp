#include "communication_service.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

#include "communication_traits.h"
#include "json_source_file.h"
#include "link_transceiver.h"
#include "link_transceiver_threaded.h"

using namespace md::app;

namespace
{
constexpr char protocol[] = "protocol";
constexpr char localPort[] = "local_port";

} // namespace

CommunicationService::CommunicationService(const QString& fileName) :
    m_protocols(),
    m_communications(),
    m_source(new data_source::JsonSourceFile(fileName)),
    m_json(m_source->read())
{
}

void CommunicationService::registerProtocol(const QString& name,
                                            md::domain::ICommunicationProtocol* protocol)
{
    for (auto& protocol : m_protocols)
    {
        if (protocol.name == name)
        {
            qCritical() << "Duplicate protocol found!";
            return;
        }
    }

    domain::ProtocolDescription protocolDescription(protocol, name, this);
    m_protocols.append(protocolDescription);

    //TODO: rewrite after sql implementation
    for (const auto& value : m_json.array())
    {
        QJsonObject communicationConfig = value.toObject();
        QString protocol = (communicationConfig.value(::protocol).toString());

        if (protocol == protocolDescription.name())
        {
            QString name = (communicationConfig.value(domain::link_parameters::name).toString());
            QString type = (communicationConfig.value(domain::link_parameters::type).toString());
            int port = (communicationConfig.value(::localPort).toInt());

            domain::LinkSpecification linkSpecification({ { domain::link_parameters::port, port },
                                                          { domain::link_parameters::type, type } },
                                                        this);

            auto communication = new domain::Communication(linkSpecification, protocolDescription,
                                                           name, this);
            communication->start();
            m_communications.insert(name, communication);
        }
    }
}
