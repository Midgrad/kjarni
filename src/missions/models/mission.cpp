#include "mission.h"

#include <QDebug>

#include "mission_traits.h"

using namespace md::domain;

Mission::Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
                 const QVariant& id, bool visible, QObject* parent) :
    NamedMixin<VisibleMixin<Entity>>(name, visible, id, parent),
    type(type),
    vehicleId(vehicleId, std::bind(&Entity::changed, this)),
    route(new MissionRoute(name, id, this))
{
}

Mission::Mission(const MissionType* type, const QVariantMap& map, QObject* parent) :
    Mission(type, map.value(props::name).toString(), map.value(props::vehicle),
            map.value(props::id), map.value(props::visible).toBool(), parent)
{
}

QVariantMap Mission::toVariantMap() const
{
    QVariantMap map = NamedMixin<VisibleMixin<Entity>>::toVariantMap();

    map.insert(props::type, this->type()->id);
    map.insert(props::vehicle, this->vehicleId());

    return map;
}

void Mission::clear()
{
    route()->clear();
}
