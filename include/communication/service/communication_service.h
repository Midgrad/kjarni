#ifndef COMMUNICATION_SERVICE_H
#define COMMUNICATION_SERVICE_H

#include <QObject>
#include <QScopedPointer>
#include <QVector>

#include "communication.h"
#include "i_communication_protocol.h"

#include "i_json_source.h"
#include "i_link_transceiver.h"
#include "i_service.h"
#include "link_factory.h"
#include "link_ptr.h"
#include "link_specification.h"
#include "link_traits.h"

namespace md::app
{
class CommunicationService
    : public QObject
    , public md::domain::IService
{
    Q_OBJECT

public:
    explicit CommunicationService(const QString& fileName);

    void registerProtocol(const QString& name, domain::ICommunicationProtocol* protocol);

private:
    QVector<domain::ProtocolDescription> m_protocols;
    QHash<QString, domain::Communication*> m_communications;

    QScopedPointer<data_source::IJsonSource> m_source;
    QJsonDocument m_json;
};

} // namespace md::app

#endif //COMMUNICATION_SERVICE_H
