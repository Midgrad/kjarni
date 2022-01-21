#ifndef COMMUNICATION_SERVICE_H
#define COMMUNICATION_SERVICE_H

#include <QObject>

#include <QVector>

#include "communication.h"
#include "communication_description.h"
#include "i_communication_protocol.h"
#include "i_json_source.h"
#include "i_service.h"
#include "protocol_specification.h"

namespace md::app
{
class CommunicationService
    : public QObject
    , public md::domain::IService
{
    Q_OBJECT

public:
    CommunicationService(const QString& fileName);

    void registerProtocol(const QString& name, data_source::ICommunicationProtocol* protocol);

private:
    void createCommunication(data_source::ICommunicationProtocol* protocol,
                             domain::ProtocolSpecification protocolSpecification);

    QVector<domain::ProtocolSpecification> m_protocolSpecifications;
    QHash<domain::CommunicationDescription*, data_source::Communication*> m_communications;
    // TODO: move to storing ProtocolDescription as a key, by implementing an < operator
    QMap<QString, data_source::ICommunicationProtocol*> m_protocols;

    QScopedPointer<data_source::IJsonSource> m_source;
    QJsonDocument m_json;
};

} // namespace md::app

#endif //COMMUNICATION_SERVICE_H
