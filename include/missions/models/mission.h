#ifndef MISSION_H
#define MISSION_H

#include "mission_route.h"
#include "mission_type.h"

namespace md::domain
{
class Mission : public Named
{
    Q_OBJECT

public:
    Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
            QObject* parent = nullptr);
    Mission(const MissionType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;

    const MissionType* type() const;
    QVariant vehicleId() const;
    MissionRoute* route() const;

public slots:
    void clear();

private:
    const MissionType* const m_type;
    MissionRoute* const m_route;

    const QVariant m_vehicleId;
};
} // namespace md::domain

#endif // MISSION_H
