#include "vehicle_type.h"

#include "vehicle_traits.h"

using namespace md::domain;

VehicleType::VehicleType(const QString& id, const QString& name, const QString& icon,
                         const QString& model) :
    id(id),
    name(name),
    icon(icon),
    model(model)
{
}

QVariantMap VehicleType::toVariantMap() const
{
    QVariantMap map;
    map.insert(props::id, id);
    map.insert(props::name, name);
    map.insert(props::icon, icon);
    map.insert(props::model, model);
    return map;
}
