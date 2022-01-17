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
#include "link_traits.h"

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

    // TODO: decide where to put it
    void checkHandlers();
    void runHandlers();

private:
    data_source::LinkPtrMap createLinks();
    void createLinkTranceivers();

    data_source::LinkPtrMap links();
    loodsman::LinkFactory* factory();

    QVector<domain::ProtocolDescription> m_protocols;
    loodsman::LinkFactory m_factory;
    data_source::LinkPtrMap m_links;
    QVector<md::domain::ILinkTransceiver*> m_linkTransceiverThreaded;
    QScopedPointer<data_source::IJsonSource> m_source;
};

} // namespace md::app

#endif //COMMUNICATION_SERVICE_H
