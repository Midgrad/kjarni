#include "comm_protocol.h"

#include "comm_links_traits.h"

using namespace md::domain;

CommProtocol::CommProtocol(const QString& id, const QString& name) : id(id), name(name)
{
}

QVariantMap CommProtocol::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, this->id());
    map.insert(props::name, this->name());
    return map;
}
