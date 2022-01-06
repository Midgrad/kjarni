#include "vehicle.h"

#include <QDebug>
#include <QMetaEnum>

#include "vehicle_traits.h"

using namespace md::domain;

Vehicle::Vehicle(const QString& type, const QString& name, const QVariant& id,
                 const QVariantMap& parameters, QObject* parent) :
    Parametrised(id, name, parameters, parent),
    type(type, std::bind(&Entity::changed, this)),
    online(false, std::bind(&Entity::changed, this))
{
}

Vehicle::Vehicle(const QVariantMap& map, QObject* parent) :
    Vehicle(map.value(props::type).toString(), map.value(props::name).toString(),
            map.value(props::id), map.value(props::params).toMap(), parent)
{
}

QVariantMap Vehicle::toVariantMap() const
{
    QVariantMap map = Parametrised::toVariantMap();
    map.insert(props::type, QVariant::fromValue(this->type()).toString());
    map.insert(props::online, this->online());
    return map;
}

void Vehicle::fromVariantMap(const QVariantMap& map)
{
    type = map.value(props::type, this->type()).toString();

    Parametrised::fromVariantMap(map);
}
