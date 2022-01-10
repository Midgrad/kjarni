#include "vehicle.h"

#include <QDebug>
#include <QMetaEnum>

#include "vehicle_traits.h"

using namespace md::domain;

Vehicle::Vehicle(const VehicleType* type, const QString& name, const QVariant& id,
                 const QVariantMap& parameters, QObject* parent) :
    Parametrised(id, name, parameters, parent),
    type(type),
    online(false, std::bind(&Entity::changed, this))
{
    Q_ASSERT(type);
}

Vehicle::Vehicle(const VehicleType* type, const QVariantMap& map, QObject* parent) :
    Vehicle(type, map.value(props::name).toString(), map.value(props::id),
            map.value(props::params).toMap(), parent)
{
}

QVariantMap Vehicle::toVariantMap() const
{
    QVariantMap map = Parametrised::toVariantMap();
    map.insert(props::type, this->type()->id);
    map.insert(props::online, this->online());
    return map;
}
