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
    virtual void saveItem(Route* route, RouteItem* item) override;
    virtual void restoreItem(Route* route, RouteItem* item) override;

private:
    Route* readRoute(const QVariant& id);
    RouteItem* readItem(const QVariant& id);
    void saveItemImpl(RouteItem* item, const QVariant& parentId, QVariantList& itemIds);
    void restoreItemImpl(RouteItem* item);

    IRoutesRepository* const m_routesRepo;
    IRouteItemsRepository* const m_itemsRepo;

    QMap<QString, const RouteType*> m_routeTypes;
    QMap<QString, const RouteItemType*> m_itemTypes; // TODO: remove
    QMap<QVariant, Route*> m_routes;

    mutable QMutex m_mutex;
};
} // namespace md::domain

#endif // ROUTES_SERVICE_H
