#include "mission_status.h"

#include <QDebug>

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
    qDebug() << "MissionStatus" << m_progress << m_total;
    return m_progress + 1 >= m_total;
}

QJsonObject MissionStatus::toJson() const
{
    QJsonObject json;
    json[params::progress] = m_progress;
    json[params::total] = m_total;
    json[params::complete] = this->isComplete();
    json[params::type] = this->type();
    return json;
}

namespace md::domain
{
bool operator==(const MissionStatus& left, const MissionStatus& right)
{
    return left.m_type == right.m_type && left.m_progress == right.m_progress &&
           left.m_total == right.m_total;
}
} // namespace md::domain
