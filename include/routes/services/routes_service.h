#ifndef ROUTES_SERVICE_H
#define ROUTES_SERVICE_H

#include "i_route_items_repository.h"
#include "i_routes_repository.h"
#include "i_routes_service.h"

#include <QMutex>

namespace md::domain
{
class RoutesService : public IRoutesService
{
    Q_OBJECT

public:
    RoutesService(IRoutesRepository* routesRepo, IRouteItemsRepository* itemsRepo,
                  QObject* parent = nullptr);

    Route* route(const QVariant& id) const override;
    QVariantList routeIds() const override;
    QList<Route*> routes() const override;
    const RouteType* routeType(const QString& id) const override;
    QList<const RouteType*> routeTypes() const override;

    RoutePattern* createRoutePattern(const QString& routePatternId) override;

    void registerRouteType(const RouteType* routeType) override;
    void unregisterRouteType(const RouteType* routeType) override;

    void registerRoutePatternFactory(const QString& routePatternId,
                                     IRoutePatternFactory* factory) override;
    void unregisterRoutePatternFactory(const QString& routePatternId) override;

    // For tests
    void addRoute(Route* route);

public slots:
    void readAll() override;
    void removeRoute(Route* route) override;
    void restoreRoute(Route* route) override;
    void saveRoute(Route* route) override;
    void saveItem(Route* route, RouteItem* item) override;
    void restoreItem(Route* route, RouteItem* item) override;

private:
    Route* readRoute(const QVariant& id);
    RouteItem* readItem(const QVariant& id);
    void saveItemImpl(RouteItem* item, const QVariant& parentId, const QVariantList& itemIds);
    void restoreItemImpl(RouteItem* item);
    void removeItems(const QVariantList& itemsIds);

    IRoutesRepository* const m_routesRepo;
    IRouteItemsRepository* const m_itemsRepo;

    QMap<QString, const RouteType*> m_routeTypes;
    QMap<QVariant, Route*> m_routes;
    QMap<QString, IRoutePatternFactory*> m_patternFactories;

    mutable QMutex m_mutex;
};
} // namespace md::domain

#endif // ROUTES_SERVICE_H
