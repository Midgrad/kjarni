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
    enum Type
    {
        NotActual,
        Downloading,
        Uploading,
        Actual
    };

    MissionStatus(Type type = NotActual, int progress = 0, int total = 0);

    Type type() const;
    int progress() const;
    int total() const;

    bool isComplete() const;
    QJsonObject toJson() const;

    friend bool operator==(const MissionStatus& left, const MissionStatus& right);

private:
    Type m_type;
    int m_progress;
    int m_total;
};

bool operator==(const MissionStatus& left, const MissionStatus& right);

} // namespace md::domain

#endif // MISSION_STATUS_H
