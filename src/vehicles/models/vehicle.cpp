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
    Parametrised(id, name, QVariantMap(), parent),
    m_type(type)
{
}

Vehicle::Vehicle(const QVariantMap& map, QObject* parent) :
    Parametrised(map, parent),
    m_type(::typeFromVariant(map.value(props::type)))
{
}

QVariantMap Vehicle::toVariantMap() const
{
    QVariantMap map = Parametrised::toVariantMap();
    map.insert(props::type, QVariant::fromValue(m_type).toString());
    return map;
}

void Vehicle::fromVariantMap(const QVariantMap& map)
{
    if (map.contains(props::type))
        m_type = ::typeFromVariant(map.value(props::type));

    Parametrised::fromVariantMap(map);
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
    emit changed();
}
