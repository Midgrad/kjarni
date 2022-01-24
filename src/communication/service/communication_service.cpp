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

CommunicationService::CommunicationService(const QString& fileName,
                                           domain::ICommunicationDescriptionRepository* repository) :
    m_protocols(),
    m_communications(),
    m_source(new data_source::JsonSourceFile(fileName)),
    m_repository(repository)
//    m_json(m_source->read())
{
    this->readAll();
}

void CommunicationService::createCommunication(data_source::ICommunicationProtocol* protocol,
                                               domain::ProtocolSpecification protocolSpecification)
{
    QMutableHashIterator<domain::CommunicationDescription*, data_source::Communication*> i(
        m_communications);
    while (i.hasNext())
    {
        i.next();

        if (i.key()->protocolSpecification().name() == protocolSpecification.name())
        {
            auto communication = new data_source::Communication(i.key()->linkSpecification(),
                                                                i.key()->protocolSpecification(),
                                                                protocol, i.key()->name, this);
            communication->start();

            i.value() = communication;

            QObject::connect(communication, &domain::ICommunication::bytesReceivedChanged, i.key(),
                             &domain::CommunicationDescription::setBytesReceived);
            QObject::connect(communication, &domain::ICommunication::bytesSentChanged, i.key(),
                             &domain::CommunicationDescription::setBytesSent);
            QObject::connect(communication, &domain::ICommunication::connectedChanged, i.key(),
                             &domain::ICommunication::setConnected);
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

    //    this->saveAll();
}

void CommunicationService::readAll()
{
    for (const QVariant& communicationDescriptionId : m_repository->selectDescriptionIds())
    {
        this->readCommunicationDescription(communicationDescriptionId);
    }
}

void CommunicationService::saveAll()
{
    QHashIterator<domain::CommunicationDescription*, data_source::Communication*> i(
        m_communications);
    while (i.hasNext())
    {
        i.next();
        m_repository->insert(i.key());
    }
}

md::domain::CommunicationDescription* CommunicationService::readCommunicationDescription(
    const QVariant& id)
{
    QVariantMap select = m_repository->select(id);
    QString type = select.value(domain::props::type).toString();

    if (type == "")
    {
        qWarning() << "Unknown communication type: " << type;
        return nullptr;
    }

    domain::ProtocolSpecification protocolSpecification(
        select.value(domain::props::protocol).toString());
    domain::LinkSpecification linkSpecification(select.value(domain::props::params).toMap());
    auto* communicationDescription =
        new domain::CommunicationDescription(linkSpecification, protocolSpecification,
                                             select.value(domain::props::state).toBool(),
                                             select.value(domain::props::id).toString(),
                                             select.value(domain::props::name).toString(), this);

    m_communications.insert(communicationDescription, nullptr);

    return communicationDescription;
}
