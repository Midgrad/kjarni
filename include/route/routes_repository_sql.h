#ifndef ROUTES_REPOSITORY_SQL_H
#define ROUTES_REPOSITORY_SQL_H

#include "entity_sql_table.h"
#include "i_routes_repository.h"

#include <QMutex>

namespace md::domain
{
class RoutesRepositorySql : public IRoutesRepository
{
    Q_OBJECT

public:
    explicit RoutesRepositorySql(QSqlDatabase* database, QObject* parent = nullptr);
    ~RoutesRepositorySql() override;

    virtual Route* route(const QVariant& id) const override;
    virtual QVariantList routeIds() const override;
    virtual QList<Route*> routes() const override;
    virtual QList<const RouteType*> routeTypes() const override;

    virtual void registerRouteType(const RouteType* routeType) override;
    virtual void unregisterRouteType(const RouteType* routeType) override;

public slots:
    virtual void readAll() override;
    virtual void removeRoute(Route* route) override;
    virtual void restoreRoute(Route* route) override;
    virtual void saveRoute(Route* route) override;

private:
    Route* readRoute(const QVariant& id);
    Waypoint* readWaypoint(const QVariant& id);
    void removeWaypoint(Waypoint* waypoint);

    data_source::EntitySqlTable m_routesTable;
    data_source::EntitySqlTable m_waypointsTable;
    data_source::SqlTable m_routeWaypointsTable;

    QMap<QString, const RouteType*> m_routeTypes;
    QMap<QString, const WaypointType*> m_waypointTypes;
    QMap<QVariant, Route*> m_routes;
    QMap<QVariant, Waypoint*> m_waypoints;
    QMultiMap<Route*, Waypoint*> m_routeWaypoints;

    mutable QMutex m_mutex;
};
} // namespace md::domain

#endif // ROUTES_REPOSITORY_SQL_H