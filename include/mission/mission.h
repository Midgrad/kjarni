#ifndef MISSION_H
#define MISSION_H

#include "mission_type.h"
#include "route.h"

namespace md::domain
{
class Mission : public Entity
{
    Q_OBJECT

public:
    Mission(const MissionType& type, const QVariant& id, const QString& name,
            QObject* parent = nullptr);
    Mission(const QJsonObject& json, const MissionType& type, QObject* parent = nullptr);

    QJsonObject toJson(bool recursive) const override;
    void fromJson(const QJsonObject& json) override;

    const MissionType& type() const;
    QString vehicle() const;
    Route* route() const;

public slots:
    void setVehicle(const QString& vehicle);

signals:
    void vehicleChanged(QString vehicle);
    void routeChanged(Route* route);

private:
    const MissionType m_type;
    QString m_vehicle;
    Route* const m_route;
};
} // namespace md::domain

#endif // MISSION_H
