#include "link_specification.h"

namespace
{
QVariantMap fillInDefaultParameters(const QVariantMap& parameters)
{
    QVariantMap return_parameters = parameters;
    if (!return_parameters.value(md::domain::link_parameters::type).isNull())
    {
        for (const QString& parameter : md::domain::link_type::parameters.values(
                 return_parameters.value(md::domain::link_parameters::type).toString()))
        {
            if (!return_parameters.contains(parameter))
            {
                return_parameters.insert(parameter,
                                         md::domain::link_type::defaultValues.value(parameter));
            }
        }
    }

    return return_parameters;
}
} // namespace

using namespace md::domain;

LinkSpecification::LinkSpecification(const QVariantMap& parameters) :
    m_type(parameters.value(link_parameters::type).toString()),
    m_parameters(::fillInDefaultParameters(parameters))
{
    Q_ASSERT((m_type == link_type::serial) || (m_type == link_type::udp) ||
             (m_type == link_type::tcp));
}

LinkSpecification::LinkSpecification(const LinkSpecification& another, QObject* parent) :
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
