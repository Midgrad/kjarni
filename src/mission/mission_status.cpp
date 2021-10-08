#include "mission_status.h"

#include "vegur_traits.h"

using namespace md::domain;

MissionStatus::MissionStatus(int progress, int total) : m_progress(progress), m_total(total)
{
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
    return m_progress >= m_total;
}

QJsonObject MissionStatus::toJson() const
{
    QJsonObject json;
    json[params::progress] = m_progress;
    json[params::total] = m_total;
    json[params::complete] = this->isComplete();
    return json;
}

namespace md::domain
{
bool operator==(const MissionStatus& left, const MissionStatus& right)
{
    return left.m_progress == right.m_progress && left.m_total == right.m_total;
}
} // namespace md::domain
