#include "mission_waypoint.h"

#include <QDebug>
#include <QMetaEnum>

#include "mission_traits.h"

using namespace md::domain;

MissionWaypoint::MissionWaypoint(State state) : m_state(state)
{
}

MissionWaypoint::State MissionWaypoint::state() const
{
    return m_state;
}

QVariantMap MissionWaypoint::toVariantMap() const
{
    QVariantMap map;
    map[params::state] = QVariant::fromValue(m_state).toString();
    return map;
}

namespace md::domain
{
bool operator==(const MissionWaypoint& left, const MissionWaypoint& right)
{
    return left.m_state == right.m_state;
}
} // namespace md::domain
