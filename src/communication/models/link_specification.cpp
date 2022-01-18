#include "link_specification.h"

using namespace md::domain;

LinkSpecification::LinkSpecification(QVariantMap parameters, QObject* parent) :
    QObject(parent),
    m_type(parameters.take(link_parameters::type).toString()),
    m_parameters(parameters)
{
    Q_ASSERT((m_type == link_type::serial) || (m_type == link_type::udp) ||
             (m_type == link_type::tcp));
    this->fillInParameters();
}

LinkSpecification::LinkSpecification(const LinkSpecification& another, QObject* parent) :
    QObject(parent),
    m_type(another.type()),
    m_parameters(another.parameters())
{
}

using namespace md::domain;

QString LinkSpecification::type() const
{
    return m_type;
}

QVariant LinkSpecification::parameter(const QString& parameter) const
{
    return m_parameters.value(parameter);
}

QVariantMap LinkSpecification::parameters() const
{
    return m_parameters;
}

void LinkSpecification::fillInParameters()
{
    if (!m_type.isNull())
    {
        for (const QString& parameter : link_type::parameters.values(m_type))
        {
            if (!m_parameters.contains(parameter))
            {
                m_parameters.insert(parameter, link_type::defaultValues.value(parameter));
            }
        }
    }
}