#include "protocol_specification.h"

using namespace md::domain;

ProtocolSpecification::ProtocolSpecification(const QString& name) : m_name(name)
{
}

QString ProtocolSpecification::name() const
{
    return m_name;
}
