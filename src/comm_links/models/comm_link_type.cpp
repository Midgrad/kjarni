#include "comm_link_type.h"
#include "comm_links_traits.h"

using namespace md::domain;

CommLinkType::CommLinkType(const QString& id, const QString& name,
                           const QVector<const ParameterType*>& parameters) :
    id(id),
    name(name),
    m_parameters(utils::vecToMap(parameters))
{
}

CommLinkType::~CommLinkType()
{
    // TODO: replace with smart ptr or store by value
    qDeleteAll(m_parameters);
}

QVariantMap CommLinkType::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, this->id());
    map.insert(props::name, this->name());
    return map;
}

const ParameterType* CommLinkType::parameter(const QString& id) const
{
    return m_parameters.value(id);
}

QList<const ParameterType*> CommLinkType::parameters() const
{
    return m_parameters.values();
}

QStringList CommLinkType::parameterKeys() const
{
    return m_parameters.keys();
}
