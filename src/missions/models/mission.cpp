#include "mission.h"

#include <QDebug>

#include "mission_traits.h"

using namespace md::domain;

Mission::Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
                 QObject* parent) :
    Named(utils::generateId(), name, parent),
    m_type(type),
    m_vehicleId(vehicleId),
    m_home(new RouteItem(type->homePointType, type->homePointType->name))
{
}

Mission::Mission(const MissionType* type, const QVariantMap& map, QObject* parent) :
    Named(map, parent),
    m_type(type),
    m_vehicleId(map.value(props::vehicle)),
    m_home(new RouteItem(type->homePointType, map.value(props::home).toMap(), this))
{
}

QVariantMap Mission::toVariantMap() const
{
    QVariantMap map = Named::toVariantMap();

    map.insert(props::type, m_type->id);
    map.insert(props::vehicle, m_vehicleId);
    map.insert(props::route, m_route->id());

    return map;
}

const MissionType* Mission::type() const
{
    return m_type;
}

QVariant Mission::vehicleId() const
{
    return m_vehicleId;
}

RouteItem* Mission::home() const
{
    return m_home;
}

Route* Mission::route() const
{
    return m_route;
}

int Mission::currentItem() const
{
    return m_currentItem;
}

void Mission::assignRoute(Route* route)
{
    if (m_route == route)
        return;

    m_route = route;
    emit routeChanged();
}

void Mission::setCurrentItem(int currentItem)
{
    if (m_currentItem == currentItem)
        return;

    m_currentItem = currentItem;
    emit currentItemChanged();
}

void Mission::clear()
{
    m_route->clear();
}
