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

Vehicle::Vehicle(Type type, const QString& name, const QVariant& id, QObject* parent) :
    Entity(id, name, QVariantMap(), parent),
    m_type(type)
{
}

Vehicle::Vehicle(const QVariantMap& map, QObject* parent) :
    Entity(map, parent),
    m_type(::typeFromVariant(map.value(params::type)))
{
}

QVariantMap Vehicle::toVariantMap(bool recursive) const
{
    Q_UNUSED(recursive)

    QVariantMap map = Entity::toVariantMap();
    map.insert(params::type, QVariant::fromValue(m_type).toString());
    return map;
}

void Vehicle::fromVariantMap(const QVariantMap& map)
{
    Entity::fromVariantMap(map);
    this->setType(::typeFromVariant(map.value(params::type)));
}

Vehicle::Type Vehicle::type() const
{
    return m_type;
}

void Vehicle::setType(Type type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(type);
}
