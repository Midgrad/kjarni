#include "protocol_description.h"

using namespace md::domain;

ProtocolDescription::ProtocolDescription(ICommunicationProtocol* communicationProtocol,
                                         const QString& name, QObject* parent) :
    Named(name, utils::generateId(), parent),
    m_communicationProtocol(communicationProtocol)
{
}

ProtocolDescription::ProtocolDescription(const ProtocolDescription& another, QObject* parent) :
    Named(another.name(), utils::generateId(), parent),
    m_communicationProtocol(another.protocol())
{
}

ProtocolDescription& ProtocolDescription::operator=(const ProtocolDescription& another)
{
    if (this == &another)
    {
        return *this;
    }

    this->m_communicationProtocol = another.protocol();
    this->name = another.name();

    return *this;
}

ICommunicationProtocol* ProtocolDescription::protocol() const
{
    return m_communicationProtocol;
}
