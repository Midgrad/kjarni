#include "protocol_description.h"

using namespace md::domain;

ProtocolDescription::ProtocolDescription(ICommunicationProtocol* communicationProtocol,
                                         const QString& name, QObject* parent) :
    Named(name, utils::generateId(), parent),
    m_communicationProtocol(communicationProtocol)
{
}

ProtocolDescription::ProtocolDescription(ProtocolDescription& another, QObject* parent) :
    Named(name, utils::generateId(), parent),
    m_communicationProtocol(another.protocol())
{
}

using namespace md::domain;

ICommunicationProtocol* ProtocolDescription::protocol() const
{
    return m_communicationProtocol;
}
