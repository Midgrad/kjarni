#include "mission_operation.h"

#include <QDebug>
#include <QMetaEnum>

#include "mission_traits.h"

using namespace md::domain;

MissionOperation::MissionOperation(QObject* parent) : QObject(parent)
{
}

MissionOperation::Type MissionOperation::type() const
{
    return m_type;
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
    QVariantMap map;
    map[props::progress] = m_progress;
    map[props::total] = m_total;
    map[props::complete] = this->isComplete();
    map[props::type] = QVariant::fromValue(m_type).toString();
    return map;
}

void MissionOperation::startDownload(int total)
{
    m_type = Downloading;
    m_total = total;
    m_progress = 0;

    emit changed();
}

void MissionOperation::startUpload(int total)
{
    m_type = Uploading;
    m_total = total;
    m_progress = 0;

    emit changed();
}

void MissionOperation::stop()
{
    if (m_type == Idle)
        return;

    m_progress = 0;
    m_total = 0;
    m_type = Idle;

    emit changed();
}

void MissionOperation::setProgress(int progress)
{
    if (m_progress == progress)
        return;

    m_progress = progress;

    if (m_progress >= m_total)
    {
        m_progress = 0;
        m_total = 0;
        m_type = Idle;
    }

    emit changed();
}
