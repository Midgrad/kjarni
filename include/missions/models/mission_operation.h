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

    MissionOperation(Type type, Mission* mission, QObject* parent = nullptr);

    utils::ConstProperty<Type> type;
    utils::ConstProperty<Mission*> mission;
    utils::Property<State> state;
    utils::Property<int> progress;
    utils::Property<int> total;

    bool isComplete() const;
    QVariantMap toVariantMap() const override;

    Q_ENUM(Type);
};

} // namespace md::domain

#endif // MISSION_OPERATION_H
