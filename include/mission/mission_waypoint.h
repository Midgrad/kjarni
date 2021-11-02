#ifndef MISSION_WAYPOINT_H
#define MISSION_WAYPOINT_H

#include <QObject>
#include <QVariantMap>

namespace md::domain
{
class MissionWaypoint
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

    MissionWaypoint(State state = Unconfirmed);

    State state() const;

    QVariantMap toVariantMap() const;

    friend bool operator==(const MissionWaypoint& left, const MissionWaypoint& right);

private:
    State m_state;

    Q_ENUM(State);
};

bool operator==(const MissionWaypoint& left, const MissionWaypoint& right);

} // namespace md::domain

#endif // MISSION_WAYPOINT_H
