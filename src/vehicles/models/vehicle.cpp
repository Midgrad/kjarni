#include "vehicle.h"

#include <QDebug>
#include <QMetaEnum>

using namespace md::domain;

namespace
{
Vehicle::Type typeFromVariant(const QVariant& value)
{
    auto&& metaEnum = QMetaEnum::fromType<Vehicle::Type>();
    return static_cast<Vehicle::Type>(metaEnum.keyToValue(value.toString().toUtf8().constData()));
}
} // namespace

Vehicle::Vehicle(Type type, const QString& name, const QVariant& id, const QVariantMap& parameters,
                 QObject* parent) :
    Parametrised(id, name, parameters, parent),
    type(type, std::bind(&Entity::changed, this))
{
}

Vehicle::Vehicle(const QVariantMap& map, QObject* parent) :
    Vehicle(::typeFromVariant(map.value(props::type)), map.value(props::name).toString(),
            map.value(props::id), map.value(props::params).toMap(), parent)
{
}

QVariantMap Vehicle::toVariantMap() const
{
    QVariantMap map = Parametrised::toVariantMap();
    map.insert(props::type, QVariant::fromValue(type.get()).toString());
    return map;
}

void Vehicle::fromVariantMap(const QVariantMap& map)
{
    if (map.contains(props::type))
        type = ::typeFromVariant(map.value(props::type));

    Parametrised::fromVariantMap(map);
}
