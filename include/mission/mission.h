#ifndef MISSION_H
#define MISSION_H

#include "mission_operation.h"
#include "mission_route.h"
#include "mission_type.h"

namespace md::domain
{
class Mission : public Entity
{
    Q_OBJECT

public:
    Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
            const QVariant& id = utils::generateId(), QObject* parent = nullptr);
    Mission(const MissionType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap(bool recursive) const override;

    const MissionType* type() const;
    QVariant vehicleId() const;
    MissionOperation* operation() const;
    MissionRoute* route() const;

public slots:
    void assignRoute(Route* route);

signals:
    void routeChanged(MissionRoute* route);

private:
    const MissionType* m_type;
    const QVariant m_vehicleId;
    MissionOperation* const m_operation;
    MissionRoute* m_route = nullptr;
};
} // namespace md::domain

#endif // MISSION_H
