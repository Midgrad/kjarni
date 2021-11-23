#ifndef ROUTES_SERVICE_H
#define ROUTES_SERVICE_H

#include "entity_sql_table.h"
#include "i_routes_service.h"

#include <QMutex>

namespace md::domain
{
class RoutesService : public IRoutesService
{
    Q_OBJECT

public:
    explicit RoutesService(QSqlDatabase* database, QObject* parent = nullptr);
    ~RoutesService() override;

    virtual Route* route(const QVariant& id) const override;
    virtual QVariantList routeIds() const override;
    virtual QList<Route*> routes() const override;
    const RouteType* routeType(const QString& id) const override;
    virtual QList<const RouteType*> routeTypes() const override;

    virtual void registerRouteType(const RouteType* routeType) override;
    virtual void unregisterRouteType(const RouteType* routeType) override;

public slots:
    virtual void readAll() override;
    virtual void removeRoute(Route* route) override;
    virtual void restoreRoute(Route* route) override;
    virtual void saveRoute(Route* route) override;
    virtual void saveWaypoint(Route* route, RouteItem* waypoint) override;
    virtual void restoreWaypoint(RouteItem* waypoint) override;

private:
    Route* readRoute(const QVariant& id);
    RouteItem* readWaypoint(const QVariant& id);
    RouteItem* readItem(const QVariant& id, const RouteItemType* wptType);
    void removeWaypoint(RouteItem* waypoint);

    data_source::EntitySqlTable m_routesTable;
    data_source::EntitySqlTable m_waypointsTable;
    data_source::EntitySqlTable m_waypointItemsTable;
    data_source::SqlTable m_routeWaypointsTable;

    QMap<QString, const RouteType*> m_routeTypes;
    QMap<QString, const RouteItemType*> m_waypointTypes;
    QMap<QVariant, Route*> m_routes;
    QMap<QVariant, RouteItem*> m_waypoints;
    QMultiMap<Route*, RouteItem*> m_routeWaypoints; // TODO: remove this

    mutable QMutex m_mutex;
};
} // namespace md::domain

#endif // ROUTES_SERVICE_H
