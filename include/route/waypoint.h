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
    Waypoint(const QString& name, const WaypointType* type);
    Waypoint(const QVariantMap& map, const WaypointType* type);

    QVariantMap toVariantMap(bool recursive) const override;
    const WaypointType* type() const;

public slots:
    void setAndCheckParameter(const QString& key, const QVariant& value);
    void resetParameter(const QString& key);
    void setType(const WaypointType* type);
    void syncParameters();

signals:
    void typeChanged();

private:
    const WaypointType* m_type;
};
} // namespace md::domain

#endif // WAYPOINT_H
