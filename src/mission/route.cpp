#include "route.h"

#include <QDebug>
#include <QJsonArray>

using namespace md::domain;

Route::Route(const RouteType* type, QObject* parent) : Entity(QString(), parent), m_type(type)
{
    Q_ASSERT(type);
}

Route::Route(const QVariantMap& map, const RouteType* type, QObject* parent) :
    Entity(map, parent),
    m_type(type)
{
    Q_ASSERT(type);

    this->fromVariantMapImpl(map);
}

QVariantMap Route::toVariantMap(bool recursive) const
{
    QVariantMap map = Entity::toVariantMap();

    map.insert(params::type, m_type->name);

    if (recursive)
    {
        QVariantList waypoints;
        for (Waypoint* waypoint : m_waypoins)
        {
            waypoints.append(waypoint->toVariantMap(recursive));
        }
        map.insert(params::waypoints, waypoints);
    }

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

int Route::count() const
{
    return m_waypoins.count();
}

const QList<Waypoint*>& Route::waypoints() const
{
    return m_waypoins;
}

Waypoint* Route::waypoint(int index) const
{
    return m_waypoins.value(index, nullptr);
}

void Route::addWaypoint(Waypoint* waypoint)
{
    if (m_waypoins.contains(waypoint))
        return;

    if (waypoint->thread() != this->thread())
        waypoint->moveToThread(this->thread());

    if (!waypoint->parent())
        waypoint->setParent(this);

    m_waypoins.append(waypoint);
    emit waypointAdded(waypoint);
}

void Route::removeWaypoint(Waypoint* waypoint)
{
    if (!m_waypoins.contains(waypoint))
        return;

    if (waypoint->parent() == this)
        waypoint->setParent(nullptr);

    m_waypoins.removeOne(waypoint);
    emit waypointRemoved(waypoint);
}

void Route::fromVariantMapImpl(const QVariantMap& map)
{
    if (map.contains(params::waypoints))
    {
        QVariantList waypoints = map.value(params::waypoints).toList();
        int counter = 0;
        for (const QVariant& value : waypoints)
        {
            QVariantMap map = value.toMap();
            QString typeName = map.value(params::type).toString();
            auto type = m_type->waypointType(typeName);
            if (!type)
            {
                qWarning() << "No waypoint type" << typeName;
                continue;
            }

            if (counter >= m_waypoins.count())
            {
                m_waypoins.append(new Waypoint(map, type, this));
            }
            else
            {
                m_waypoins[counter]->fromVariantMap(map);
            }
            counter++;
        }

        // Remove tail from old route
        while (m_waypoins.count() > waypoints.count())
        {
            this->removeWaypoint(m_waypoins.last());
        }
    }
}
