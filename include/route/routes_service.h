#ifndef ROUTES_SERVICE_H
#define ROUTES_SERVICE_H

#include "i_repository_factory.h"
#include "i_routes_service.h"

#include <QMutex>

namespace md::domain
{
class RoutesService : public IRoutesService
{
    Q_OBJECT

public:
    explicit RoutesService(IRepositoryFactory* repoFactory, QObject* parent = nullptr);
    ~RoutesService() override;

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

    const QScopedPointer<IEntityRepository> m_routesRepo;
    const QScopedPointer<IEntityRepository> m_waypointsRepo;
    QMap<QString, const RouteType*> m_routeTypes;
    QMap<QString, const WaypointType*> m_waypointTypes;
    QMap<QVariant, Route*> m_routes;
    QMap<QVariant, Waypoint*> m_waypoints;
    QMultiMap<Route*, Waypoint*> m_routeWaypoints;

    mutable QMutex m_mutex;
};
} // namespace md::domain

#endif // ROUTES_SERVICE_H
