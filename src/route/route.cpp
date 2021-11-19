#include "route.h"

#include <QDebug>
#include <QJsonArray>

#include "route_traits.h"

using namespace md::domain;

Route::Route(const RouteType* type, const QString& name, const QVariant& id, QObject* parent) :
    Entity(id, name, QVariantMap(), parent),
    m_type(type)
{
    Q_ASSERT(type);
}

Route::Route(const RouteType* type, const QVariantMap& map, QObject* parent) :
    Entity(map, parent),
    m_type(type)
{
    Q_ASSERT(type);

    this->fromVariantMapImpl(map);
}

QVariantMap Route::toVariantMap() const
{
    QVariantMap map = Entity::toVariantMap();

    map.insert(props::type, m_type->id);

    return map;
}

void Route::fromVariantMap(const QVariantMap& map)
{
    this->fromVariantMapImpl(map);

    Entity::fromVariantMap(map);
}

const RouteType* Route::type() const
{
    return m_type;
}

int Route::waypointsCount() const
{
    return m_waypoints.count();
}

int Route::waypointIndex(Waypoint* waypoint) const
{
    return m_waypoints.indexOf(waypoint);
}

const QList<Waypoint*>& Route::waypoints() const
{
    return m_waypoints;
}

Waypoint* Route::waypoint(int index) const
{
    return m_waypoints.value(index, nullptr);
}

int Route::itemsCount() const
{
    int count = 0;
    for (Waypoint* waypoint : qAsConst(m_waypoints))
    {
        count += waypoint->count() + 1; // Calc self
    }
    return count;
}

QList<WaypointItem*> Route::items() const
{
    QList<WaypointItem*> items;
    for (Waypoint* waypoint : qAsConst(m_waypoints))
    {
        items.append(waypoint);
        items += waypoint->items();
    }

    return items;
}

WaypointItem* Route::item(int index) const
{
    int count = 0;
    for (Waypoint* waypoint : qAsConst(m_waypoints))
    {
        if (count == index)
            return waypoint;

        count++;
        for (WaypointItem* item : waypoint->items())
        {
            if (count == index)
                return item;
            count++;
        }
    }
    return nullptr;
}

void Route::setWaypoints(const QList<Waypoint*>& waypoints)
{
    // Remove old waypoints (std::remove_if does not emit signals)
    for (Waypoint* waypoint : qAsConst(m_waypoints))
    {
        // Skip waypoint if we have it in new list
        if (waypoints.contains(waypoint))
            continue;

        this->removeWaypoint(waypoint);
    }

    // Add new waypoints to the end
    for (Waypoint* waypoint : waypoints)
    {
        this->addWaypoint(waypoint);
    }
}

void Route::addWaypoint(Waypoint* waypoint)
{
    if (m_waypoints.contains(waypoint))
        return;

    if (waypoint->thread() != this->thread())
        waypoint->moveToThread(this->thread());

    if (!waypoint->parent())
        waypoint->setParent(this);

    connect(waypoint, &WaypointItem::changed, this, [waypoint, this]() {
        emit waypointChanged(this->waypointIndex(waypoint), waypoint);
    });

    m_waypoints.append(waypoint);
    emit waypointAdded(m_waypoints.count() - 1, waypoint);
}

void Route::removeWaypoint(Waypoint* waypoint)
{
    int index = m_waypoints.indexOf(waypoint);
    // Remove but don't TODO: delete waypoint
    if (index == -1)
        return;

    if (waypoint->parent() == this)
        waypoint->setParent(nullptr);

    disconnect(waypoint, nullptr, this, nullptr);

    m_waypoints.removeOne(waypoint);
    emit waypointRemoved(index, waypoint);
}

void Route::clear()
{
    for (Waypoint* waypoint : m_waypoints)
    {
        this->removeWaypoint(waypoint);
    }
}

void Route::fromVariantMapImpl(const QVariantMap& map)
{
    if (map.contains(props::waypoints))
    {
        QVariantList waypoints = map.value(props::waypoints).toList();
        int counter = 0;
        for (const QVariant& value : waypoints)
        {
            QVariantMap map = value.toMap();
            QString typeName = map.value(props::type, tr("empty")).toString();
            auto type = m_type->waypointType(typeName);
            if (!type)
            {
                qWarning() << "No waypoint type" << typeName;
                continue;
            }

            if (counter >= m_waypoints.count())
            {
                auto waypoint = new Waypoint(type, map);
                waypoint->moveToThread(this->thread());
                waypoint->setParent(this);

                m_waypoints.append(waypoint);
            }
            else
            {
                m_waypoints[counter]->fromVariantMap(map);
            }
            counter++;
        }

        // Remove tail from old route
        while (m_waypoints.count() > waypoints.count())
        {
            this->removeWaypoint(m_waypoints.last());
        }
    }
}
