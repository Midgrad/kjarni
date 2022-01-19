#include "protocol_specification.h"

using namespace md::domain;

ProtocolSpecification::ProtocolSpecification(const QString& name, QObject* parent) :
    QObject(parent),
    m_name(name)
{
}

ProtocolSpecification::ProtocolSpecification(const ProtocolSpecification& another, QObject* parent) :
    QObject(parent),
    m_name(another.name())
{
}

ProtocolSpecification& ProtocolSpecification::operator=(const ProtocolSpecification& another)
{
    if (this == &another)
    {
        return *this;
    }

    this->m_name = another.name();

    return *this;
}

QString ProtocolSpecification::name() const
{
    return m_name;
}
