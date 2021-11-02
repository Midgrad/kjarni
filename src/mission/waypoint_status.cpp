#include "waypoint_status.h"

#include <QDebug>
#include <QMetaEnum>

#include "mission_traits.h"

using namespace md::domain;

WaypointStatus::WaypointStatus(State state) : m_state(state)
{
}

WaypointStatus::State WaypointStatus::state() const
{
    return m_state;
}

QVariantMap WaypointStatus::toVariantMap() const
{
    QVariantMap map;
    map[params::state] = QVariant::fromValue(m_state).toString();
    return map;
}

namespace md::domain
{
bool operator==(const WaypointStatus& left, const WaypointStatus& right)
{
    return left.m_state == right.m_state;
}
} // namespace md::domain
