#ifndef WAYPOINT_STATUS_H
#define WAYPOINT_STATUS_H

#include <QObject>
#include <QVariantMap>

namespace md::domain
{
class WaypointStatus // TODO: MissionWaypoint
{
    Q_GADGET

public:
    enum State
    {
        Unknown,
        Current,
        Normal,
        Reached,
        Unconfirmed
    };

    WaypointStatus(State state = Unconfirmed);

    State state() const;

    QVariantMap toVariantMap() const;

    friend bool operator==(const WaypointStatus& left, const WaypointStatus& right);

private:
    State m_state;

    Q_ENUM(State);
};

bool operator==(const WaypointStatus& left, const WaypointStatus& right);

} // namespace md::domain

#endif // WAYPOINT_STATUS_H
