#include "route.h"

#include <QDebug>
#include <QJsonArray>

using namespace md::domain;

Route::Route(const RouteType* routeType, QObject* parent) :
    Entity(QString(), parent),
    m_routeType(routeType)
{
    Q_ASSERT(routeType);
}

Route::Route(const QJsonObject& json, const RouteType* routeType, QObject* parent) :
    Entity(json, parent),
    m_routeType(routeType)
{
    Q_ASSERT(routeType);

    this->fromJsonImpl(json);
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
    this->fromJsonImpl(json);

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

void Route::fromJsonImpl(const QJsonObject& json)
{
    if (json.contains(params::waypoints))
    {
        QJsonArray waypoints = json.value(params::waypoints).toArray();
        int counter = 0;
        for (const QJsonValue& value : waypoints)
        {
            QJsonObject json = value.toObject();
            QString typeName = json.value(params::type).toString();
            auto type = m_routeType->waypointType(typeName);
            if (!type)
            {
                qWarning() << "No waypoint type" << typeName;
                continue;
            }

            if (counter >= m_waypoins.count())
            {
                m_waypoins.append(new Waypoint(json, type, this));
            }
            else
            {
                m_waypoins[counter]->fromJson(json);
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
