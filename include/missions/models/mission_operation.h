#ifndef MISSION_OPERATION_H
#define MISSION_OPERATION_H

#include "mission.h"

namespace md::domain
{
class MissionOperation : public Entity
{
    Q_OBJECT

public:
    enum Type
    {
        Download,
        Upload,
        Clear
    };

    enum State
    {
        InProgress,
        Succeeded,
        Failed
    };

    MissionOperation(Type type, Mission* mission, QObject* parent);

    Type type() const;
    Mission* mission() const;
    State state() const;
    int progress() const;
    int total() const;

    bool isComplete() const;
    QVariantMap toVariantMap() const override;

public slots:
    void setState(State state);
    void setProgress(int progress);
    void setTotal(int total);

private:
    const Type m_type;
    Mission* const m_mission;
    State m_state = InProgress;
    int m_progress = 0;
    int m_total = 0;

    Q_ENUM(Type);
};

} // namespace md::domain

#endif // MISSION_OPERATION_H
