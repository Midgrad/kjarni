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
    Waypoint(const QString& name, WaypointType* type, QObject* parent = nullptr);
    Waypoint(const QJsonObject& json, WaypointType* type, QObject* parent = nullptr);

    QJsonObject toJson(bool recursive) const override;
    WaypointType* type() const;

public slots:
    void setType(WaypointType* type);

signals:
    void typeChanged();

private:
    WaypointType* m_type;
};
} // namespace md::domain

#endif // WAYPOINT_H
