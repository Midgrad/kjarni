#include "comm_link_type.h"
#include "comm_links_traits.h"

using namespace md::domain;

CommLinkType::CommLinkType(const QString& id, const QString& name) : id(id), name(name)
{
}

QVariantMap CommLinkType::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, id);
    map.insert(props::name, name);
    return map;
}
