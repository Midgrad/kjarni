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
    Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
            const QVariant& id = utils::generateId(), QObject* parent = nullptr);
    Mission(const MissionType* type, const QVariantMap& map, QObject* parent = nullptr);

    utils::ConstProperty<MissionType const*> type;
    utils::ConstProperty<QVariant> vehicleId;
    utils::ConstProperty<MissionRoute*> route;

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

public slots:
    void clear();

signals:
    void goTo(int item);
    void navTo(double latitude, double longitude, float altitude);
};
} // namespace md::domain

#endif // MISSION_H
