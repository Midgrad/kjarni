#ifndef PROTOCOL_DESCRIPTION_H
#define PROTOCOL_DESCRIPTION_H

#include "i_communication_protocol.h"
#include "named.h"

#include <QObject>
#include <QString>

namespace md::domain
{
class ProtocolDescription : public Named
{
    Q_OBJECT

public:
    explicit ProtocolDescription() = default;
    explicit ProtocolDescription(ICommunicationProtocol* communicationProtocol,
                                 const QString& name = QString(), QObject* parent = nullptr);
    ProtocolDescription(ProtocolDescription& another, QObject* parent = nullptr);

    ICommunicationProtocol* protocol() const;

private:
    ICommunicationProtocol* const m_communicationProtocol;
};
} // namespace md::domain

#endif //PROTOCOL_DESCRIPTION_H