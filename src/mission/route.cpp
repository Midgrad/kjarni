#include "route.h"

#include <QJsonArray>

using namespace md::domain;

Route::Route(QObject* parent) : Entity(QString(), parent)
{
}

Route::Route(const QJsonObject& json, QObject* parent) : Entity(json, parent)
{
    QJsonArray waypoints = json.value(params::waypoints).toArray();
    for (const QJsonValue& value : waypoints)
    {
        m_waypoins.append(new Waypoint(value.toObject(), this));
    }
}

QJsonObject Route::toJson(bool recursive) const
{
    QJsonObject json = Entity::toJson();

    if (recursive)
    {
        QJsonArray waypoints;
        for (Waypoint* waypoint : m_waypoins)
        {
            waypoints.append(waypoint->toJson(recursive));
        }

        json.insert(params::waypoints, waypoints);
    }

    return json;
}

void Route::fromJson(const QJsonObject& json)
{
    if (json.contains(params::waypoints))
    {
        QJsonArray waypoints = json.value(params::waypoints).toArray();
        int counter = 0;
        for (const QJsonValue& value : waypoints)
        {
            if (counter > m_waypoins.count())
            {
                m_waypoins.append(new Waypoint(value.toObject(), this));
            }
            else
            {
                m_waypoins[counter]->fromJson(value.toObject());
            }
            counter++;
        }

        // Remove tail from old route
        while (m_waypoins.count() > waypoints.count())
        {
            this->removeWaypoint(m_waypoins.last());
        }
    }

    Entity::fromJson(json);
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
