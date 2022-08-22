#include "comm_link.h"

#include <QDebug>
#include <QMetaEnum>

#include "comm_links_traits.h"

using namespace md::domain;

CommLink::CommLink(const CommLinkType* type, const QString& name, const QVariant& id,
                   const QString& protocol, const QVariantMap& params, QObject* parent) :
    TypedParametrisedMixin<NamedMixin<Entity>>(type->parameters().toVector(), params, name, id,
                                               parent),
    type(type),
    protocol(protocol, std::bind(&Entity::changed, this)),
    connected(false, std::bind(&Entity::changed, this)),
    online(false, std::bind(&Entity::changed, this)),
    m_type(type)
{
    Q_ASSERT(type);
}

CommLink::CommLink(const CommLinkType* type, const QVariantMap& map, QObject* parent) :
    CommLink(type, map.value(props::name).toString(), map.value(props::id),
             map.value(props::protocol).toString(), map.value(props::params).toMap(), parent)
{
}

QVariantMap CommLink::toVariantMap() const
{
    QVariantMap map = TypedParametrisedMixin<NamedMixin<Entity>>::toVariantMap();

    map.insert(props::type, this->type()->id());
    map.insert(props::protocol, this->protocol());
    map.insert(props::autoconnect, this->autoconnect());

    map.insert(props::connected, this->connected());
    map.insert(props::online, this->online());

    return map;
}

void CommLink::setType(const CommLinkType* type)
{
    Q_ASSERT(type);

    if (m_type == type)
        return;

    m_type = type;

    this->resertToDefaultParameters();
}

void CommLink::resertToDefaultParameters()
{
    this->resetTypeParameters(m_type->parameters().toVector());
}
