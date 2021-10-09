#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "entity.h"
#include "waypoint_type.h"

namespace md::domain
{
class Waypoint : public Entity
{
    Q_OBJECT

public:
    Waypoint(const QString& name, const WaypointType* type, QObject* parent = nullptr);
    Waypoint(const QJsonObject& json, const WaypointType* type, QObject* parent = nullptr);

    QJsonObject toJson(bool recursive) const override;
    const WaypointType* type() const;

public slots:
    void setType(const WaypointType* type);

signals:
    void typeChanged();

private:
    const WaypointType* m_type;
};
} // namespace md::domain

#endif // WAYPOINT_H
