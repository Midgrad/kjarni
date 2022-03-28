#ifndef MISSION_H
#define MISSION_H

#include "mission_route.h"
#include "mission_type.h"

namespace md::domain
{
class Mission : public NamedMixin<Entity>
{
    Q_OBJECT

public:
    Mission(const MissionType* type, const QString& name, const QVariant& vehicleId = QVariant(),
            const QVariant& id = utils::generateId(), const QVariant& homeId = utils::generateId(),
            QObject* parent = nullptr);
    Mission(const MissionType* type, const QVariantMap& map, QObject* parent = nullptr);

    utils::ConstProperty<MissionType const*> type;
    utils::Property<QVariant> vehicleId;
    utils::ConstProperty<MissionRoute*> route;

    QVariantMap toVariantMap() const override;

public slots:
    void clear();

signals:
    // TODO: to vehicle
    void navTo(double latitude, double longitude, float altitude);

private:
    MissionRouteItem* m_currentItem = nullptr;
};
} // namespace md::domain

#endif // MISSION_H
