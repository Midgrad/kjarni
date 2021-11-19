#ifndef MISSION_H
#define MISSION_H

#include "mission_operation.h"
#include "mission_route.h"

namespace md::domain
{
class Mission : public Entity
{
    Q_OBJECT

public:
    Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
            const QVariant& id = utils::generateId(), QObject* parent = nullptr);
    Mission(const MissionType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;

    const MissionType* type() const;
    QVariant vehicleId() const;
    MissionOperation* operation() const;
    MissionRoute* route() const;

private:
    const MissionType* const m_type;
    const QVariant m_vehicleId;
    MissionOperation* const m_operation; // TODO: mision operation to service
    MissionRoute* const m_route;
};
} // namespace md::domain

#endif // MISSION_H
