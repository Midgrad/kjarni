#ifndef I_ROUTES_SERVICE_H
#define I_ROUTES_SERVICE_H

#include "i_service.h"
#include "route.h"

namespace md::domain
{
class IRoutesService
    : public QObject
    , public IService
{
    Q_OBJECT

public:
    IRoutesService(QObject* parent) : QObject(parent), IService()
    {
    }

    virtual Route* route(const QVariant& id) const = 0;
    virtual QVariantList routeIds() const = 0;
    virtual QList<Route*> routes() const = 0;
    virtual QList<const RouteType*> routeTypes() const = 0;

    virtual void registerRouteType(const RouteType* wptType) = 0;
    virtual void unregisterRouteType(const RouteType* wptType) = 0;

public slots:
    virtual void readAll() = 0;
    virtual void removeRoute(Route* route) = 0;
    virtual void restoreRoute(Route* route) = 0;
    virtual void saveRoute(Route* route) = 0;

signals:
    void routeTypesChanged();

    void routeAdded(Route* route);
    void routeChanged(Route* route);
    void routeRemoved(Route* route);
};
} // namespace md::domain

#endif // I_ROUTES_SERVICE_H