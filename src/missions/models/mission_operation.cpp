#include "mission_operation.h"

#include <QDebug>
#include <QMetaEnum>

#include "mission_traits.h"

using namespace md::domain;

MissionOperation::MissionOperation(Type type, Mission* mission, QObject* parent) :
    Entity(mission->id(), parent),
    m_type(type),
    m_mission(mission)
{
    Q_ASSERT(mission);
}

MissionOperation::Type MissionOperation::type() const
{
    return m_type;
}

Mission* MissionOperation::mission() const
{
    return m_mission;
}

MissionOperation::State MissionOperation::state() const
{
    return m_state;
}

int MissionOperation::progress() const
{
    return m_progress;
}

int MissionOperation::total() const
{
    return m_total;
}

bool MissionOperation::isComplete() const
{
    return m_progress >= m_total;
}

QVariantMap MissionOperation::toVariantMap() const
{
    QVariantMap map = Entity::toVariantMap();
    map[props::progress] = m_progress;
    map[props::total] = m_total;
    map[props::complete] = this->isComplete();
    map[props::type] = QVariant::fromValue(m_type).toString();
    return map;
}

void MissionOperation::setState(State state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit changed();
}

void MissionOperation::setProgress(int progress)
{
    if (m_progress == progress)
        return;

    m_progress = progress;
    emit changed();
}

void MissionOperation::setTotal(int total)
{
    if (m_total == total)
        return;

    m_total = total;
    emit changed();
}
