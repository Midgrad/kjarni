#include "comm_link.h"

#include <QDebug>
#include <QMetaEnum>

#include "comm_links_traits.h"

using namespace md::domain;

CommLink::CommLink(const CommLinkType* type, const QString& name, const QVariant& id,
                   const QString& protocol, const QVariantMap& parameters, QObject* parent) :
    ParametrisedMixin<NamedMixin<Entity>>(parameters, std::bind(&Entity::changed, this), name, id,
                                          parent),
    type(type),
    protocol(protocol, std::bind(&Entity::changed, this)),
    connected(false, std::bind(&Entity::changed, this)),
    online(false, std::bind(&Entity::changed, this))
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
    QVariantMap map = ParametrisedMixin<NamedMixin<Entity>>::toVariantMap();

    map.insert(props::type, this->type()->id);
    map.insert(props::protocol, this->protocol());
    map.insert(props::connected, this->protocol());
    map.insert(props::online, this->protocol());

    return map;
}
