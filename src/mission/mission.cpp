#include "mission.h"

#include <QDebug>

#include "mission_traits.h"

using namespace md::domain;

Mission::Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
                 const QVariant& id, QObject* parent) :
    Entity(id, name, QVariantMap(), parent),
    m_type(type),
    m_vehicleId(vehicleId),
    m_operation(new MissionOperation(this)),
    m_homePoint(new Waypoint(type->homePointType))
{
}

Mission::Mission(const MissionType* type, const QVariantMap& map, QObject* parent) :
    Entity(map, parent),
    m_type(type),
    m_vehicleId(map.value(props::vehicle)),
    m_operation(new MissionOperation(this)),
    m_homePoint(new Waypoint(type->homePointType, map.value(props::home).toMap()))
{
    m_homePoint->setName(tr("Home"));
}

QVariantMap Mission::toVariantMap() const
{
    QVariantMap map = Entity::toVariantMap();

    map.insert(props::type, m_type->id);
    map.insert(props::vehicle, m_vehicleId);
    map.insert(props::home, m_homePoint->id());

    if (m_route)
    {
        map.insert(props::route, m_route->id());
    }

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

MissionOperation* Mission::operation() const
{
    return m_operation;
}

Waypoint* Mission::homePoint() const
{
    return m_homePoint;
}

Route* Mission::route() const
{
    return m_route;
}

int Mission::currentItem() const
{
    return m_currentItem;
}

int Mission::count()
{
    return m_route ? m_route->itemsCount() + 1 : 1; // route items (wpt + payload) + home point
}

WaypointItem* Mission::item(int index) const
{
    if (index == 0)
        return m_homePoint;

    return m_route ? m_route->item(index - 1) : nullptr;
}

QList<WaypointItem*> Mission::items() const
{
    QList<WaypointItem*> items;
    items.append(m_homePoint);

    if (m_route)
        items.append(m_route->items());

    return items;
}

void Mission::assignRoute(Route* route)
{
    if (m_route == route)
        return;

    if (m_route)
    {
        disconnect(m_route, nullptr, this, nullptr);
    }

    m_route = route;

    if (m_route)
    {
        connect(m_route, &Route::waypointAdded, this, &Mission::itemsChanged);
        connect(m_route, &Route::waypointChanged, this, &Mission::itemsChanged);
        connect(m_route, &Route::waypointRemoved, this, &Mission::itemsChanged);
    }

    emit routeChanged(m_route);
    emit itemsChanged();
}

void Mission::setCurrentItem(int currentItem)
{
    if (m_currentItem == currentItem)
        return;

    m_currentItem = currentItem;
    emit currentItemChanged(m_currentItem);
}
