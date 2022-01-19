#ifndef COMMUNICATION_SERVICE_H
#define COMMUNICATION_SERVICE_H

#include <QObject>
#include <QScopedPointer>
#include <QVector>

#include "i_service.h"

#include "communication.h"
#include "i_communication_protocol.h"
#include "i_json_source.h"

namespace md::app
{
class CommunicationService
    : public QObject
    , public md::domain::IService
{
    Q_OBJECT

public:
    CommunicationService(const QString& fileName);

    void registerProtocol(const QString& name, domain::ICommunicationProtocol* protocol);

private:
    QVector<domain::ProtocolDescription> m_protocols;
    QHash<QString, data_source::Communication*> m_communications;

    QScopedPointer<data_source::IJsonSource> m_source;
    QJsonDocument m_json;
};

} // namespace md::app

#endif //COMMUNICATION_SERVICE_H
