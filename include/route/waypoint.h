#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "entity.h"
#include "waypoint_type.h"

namespace md::domain
{
class Route;
class Waypoint : public Entity
{
    Q_OBJECT

public:
    Waypoint(const QString& name, const WaypointType* type, QObject* parent = nullptr);
    Waypoint(const QVariantMap& map, const WaypointType* type, QObject* parent = nullptr);

    QVariantMap toVariantMap(bool recursive) const override;
    const WaypointType* type() const;
    Route* route() const;

public slots:
    void setAndCheckParameter(const QString& key, const QVariant& value);
    void resetParameter(const QString& key);
    void setType(const WaypointType* type);
    void setRoute(Route* route);
    void syncParameters();

signals:
    void typeChanged();
    void routeChanged(Route* route);

private:
    const WaypointType* m_type;
    Route* m_route = nullptr;
};
} // namespace md::domain

#endif // WAYPOINT_H
