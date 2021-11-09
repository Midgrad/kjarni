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

    map.insert(params::type, m_type->id);

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

int Route::index(Waypoint* waypoint) const
{
    return m_waypoins.indexOf(waypoint);
}

const QList<Waypoint*>& Route::waypoints() const
{
    return m_waypoins;
}

Waypoint* Route::waypoint(int index) const
{
    return m_waypoins.value(index, nullptr);
}

void Route::setWaypoints(const QList<Waypoint*>& waypoins)
{
    // Remove old waypoints (std::remove_if does not emit signals)
    for (Waypoint* waypoint : qAsConst(m_waypoins))
    {
        // Skip waypoint if we have it in new list
        if (waypoins.contains(waypoint))
            continue;

        this->removeWaypoint(waypoint);
    }

    // Add new waypoints to the end
    for (Waypoint* waypoint : waypoins)
    {
        this->addWaypoint(waypoint);
    }
}

void Route::addWaypoint(Waypoint* waypoint)
{
    if (m_waypoins.contains(waypoint))
        return;

    if (waypoint->thread() != this->thread())
        waypoint->moveToThread(this->thread());

    if (!waypoint->parent())
        waypoint->setParent(this);

    connect(waypoint, &Waypoint::changed, this, [waypoint, this]() {
        emit waypointChanged(this->index(waypoint), waypoint);
    });

    m_waypoins.append(waypoint);
    emit waypointAdded(m_waypoins.count() - 1, waypoint);
}

void Route::removeWaypoint(Waypoint* waypoint)
{
    int index = m_waypoins.indexOf(waypoint);
    // Remove but don't delete waypoint
    if (index == -1)
        return;

    if (waypoint->parent() == this)
        waypoint->setParent(nullptr);

    disconnect(waypoint, nullptr, this, nullptr);

    m_waypoins.removeOne(waypoint);
    emit waypointRemoved(index, waypoint);
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
            QString typeName = map.value(params::type, tr("empty")).toString();
            auto type = m_type->waypointType(typeName);
            if (!type)
            {
                qWarning() << "No waypoint type" << typeName;
                continue;
            }

            if (counter >= m_waypoins.count())
            {
                auto waypoint = new Waypoint(type, map);
                waypoint->moveToThread(this->thread());
                waypoint->setParent(this);

                m_waypoins.append(waypoint);
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
