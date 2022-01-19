#include "link_specification.h"

namespace
{
void fillInParameters(QVariantMap& parameters)
{
    if (!parameters.value(md::domain::link_parameters::type).isNull())
    {
        for (const QString& parameter : md::domain::link_type::parameters.values(
                 parameters.value(md::domain::link_parameters::type).toString()))
        {
            if (!parameters.contains(parameter))
            {
                parameters.insert(parameter, md::domain::link_type::defaultValues.value(parameter));
            }
        }
    }
}
} // namespace

using namespace md::domain;

LinkSpecification::LinkSpecification(const QVariantMap& parameters, QObject* parent) :
    QObject(parent),
    m_type(parameters.value(link_parameters::type).toString()),
    m_parameters(parameters)
{
    Q_ASSERT((m_type == link_type::serial) || (m_type == link_type::udp) ||
             (m_type == link_type::tcp));
    ::fillInParameters(m_parameters);
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
