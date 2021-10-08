#ifndef MISSION_STATUS_H
#define MISSION_STATUS_H

#include <QJsonObject>
#include <QObject>

namespace md::domain
{
class MissionStatus
{
    Q_GADGET

public:
    MissionStatus(int progress = 0, int total = 0);

    int progress() const;
    int total() const;

    bool isComplete() const;
    QJsonObject toJson() const;

    friend bool operator==(const MissionStatus& left, const MissionStatus& right);

private:
    int m_progress;
    int m_total;
};

bool operator==(const MissionStatus& left, const MissionStatus& right);

} // namespace md::domain

#endif // MISSION_STATUS_H
