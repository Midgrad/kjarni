#include "mission_status.h"

#include <QDebug>
#include <QMetaEnum>

#include "mission_traits.h"

using namespace md::domain;

MissionStatus::MissionStatus(Type type, int progress, int total) :
    m_type(type),
    m_progress(progress),
    m_total(total)
{
}

MissionStatus::Type MissionStatus::type() const
{
    return m_type;
}

int MissionStatus::progress() const
{
    return m_progress;
}

int MissionStatus::total() const
{
    return m_total;
}

bool MissionStatus::isComplete() const
{
    return m_progress + 1 >= m_total;
}

QVariantMap MissionStatus::toVariantMap() const
{
    QVariantMap map;
    map[params::progress] = m_progress;
    map[params::total] = m_total;
    map[params::complete] = this->isComplete();
    map[params::type] = QVariant::fromValue(m_type).toString();
    return map;
}

namespace md::domain
{
bool operator==(const MissionStatus& left, const MissionStatus& right)
{
    return left.m_type == right.m_type && left.m_progress == right.m_progress &&
           left.m_total == right.m_total;
}
} // namespace md::domain
