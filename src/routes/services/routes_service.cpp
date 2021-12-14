#include "routes_service.h"

#include <QDebug>

#include "route_traits.h"
#include "utils.h"

using namespace md::domain;

RoutesService::RoutesService(IRoutesRepository* routesRepo, IRouteItemsRepository* itemsRepo,
                             QObject* parent) :
    IRoutesService(parent),
    m_routesRepo(routesRepo),
    m_itemsRepo(itemsRepo),
    m_mutex(QMutex::Recursive)
{
}

Route* RoutesService::route(const QVariant& id) const
{
    QMutexLocker locker(&m_mutex);
    return m_routes.value(id, nullptr);
}

QVariantList RoutesService::routeIds() const
{
    QMutexLocker locker(&m_mutex);
    return m_routes.keys();
}

QList<Route*> RoutesService::routes() const
{
    QMutexLocker locker(&m_mutex);
    return m_routes.values();
}

const RouteType* RoutesService::routeType(const QString& id) const
{
    QMutexLocker locker(&m_mutex);
    return m_routeTypes.value(id, nullptr);
}

QList<const RouteType*> RoutesService::routeTypes() const
{
    QMutexLocker locker(&m_mutex);
    return m_routeTypes.values();
}

void RoutesService::registerRouteType(const RouteType* routeType)
{
    QMutexLocker locker(&m_mutex);

    if (m_routeTypes.contains(routeType->id))
        return;

    // Route type
    m_routeTypes.insert(routeType->id, routeType);

    // Route item types
    for (const RouteItemType* itemType : routeType->itemTypes)
    {
        m_itemTypes.insert(itemType->id, itemType);
    }

    emit routeTypesChanged();
}

void RoutesService::unregisterRouteType(const RouteType* routeType)
{
    QMutexLocker locker(&m_mutex);

    if (!m_routeTypes.contains(routeType->id))
        return;

    // Route item types
    for (const RouteItemType* itemType : routeType->itemTypes)
    {
        m_itemTypes.remove(itemType->id);
    }

    // Route type
    m_routeTypes.remove(routeType->id);

    emit routeTypesChanged();
}

void RoutesService::addRoute(Route* route)
{
    route->setParent(this);
    m_routes.insert(route->id, route);
}

void RoutesService::readAll()
{
    QMutexLocker locker(&m_mutex);

    for (const QVariant& routeId : m_routesRepo->selectRouteIds())
    {
        if (!m_routes.contains(routeId))
        {
            this->readRoute(routeId);
        }
    }
}

void RoutesService::removeRoute(Route* route)
{
    QMutexLocker locker(&m_mutex);

    // Delete items first
    this->removeItems(m_itemsRepo->selectChildItemsIds(route->id));

    // Delete route
    m_routesRepo->remove(route);
    m_routes.remove(route->id);
    emit routeRemoved(route);

    route->deleteLater();
}

void RoutesService::restoreRoute(Route* route)
{
    QMutexLocker locker(&m_mutex);

    QVariantList itemIds = m_itemsRepo->selectChildItemsIds(route->id);
    for (RouteItem* item : route->items())
    {
        // Restore stored item
        if (itemIds.contains(item->id))
        {
            this->restoreItemImpl(item);
            itemIds.removeOne(item->id);
        }
        // Remove newbie items
        else
        {
            route->removeItem(item);
        }
    }

    // Read removed items
    for (const QVariant& itemId : itemIds)
    {
        auto item = this->readItem(itemId);
        if (item)
            route->addItem(item); // TODO: valid index
    }

    // Finaly restore route
    m_routesRepo->read(route);
    emit routeChanged(route);
}

void RoutesService::saveRoute(Route* route)
{
    QMutexLocker locker(&m_mutex);
    bool added;

    // Update or insert route
    if (m_routes.contains(route->id))
    {
        m_routesRepo->update(route);
        added = false;
    }
    else
    {
        m_routesRepo->insert(route);
        m_routes.insert(route->id, route);

        route->moveToThread(this->thread());
        route->setParent(this);
        added = true;
    }

    // Update or insert items
    QVariantList itemIds = m_itemsRepo->selectChildItemsIds(route->id);
    for (RouteItem* item : route->items())
    {
        this->saveItemImpl(item, route->id, itemIds);
        itemIds.removeOne(item->id);
    }

    // Delete removed items
    this->removeItems(itemIds);

    added ? emit routeAdded(route) : emit routeChanged(route);
}

void RoutesService::saveItem(Route* route, RouteItem* item)
{
    QMutexLocker locker(&m_mutex);

    this->saveItemImpl(item, route->id, m_itemsRepo->selectChildItemsIds(route->id));

    emit routeChanged(route);
}

void RoutesService::restoreItem(Route* route, RouteItem* item)
{
    QMutexLocker locker(&m_mutex);

    this->restoreItemImpl(item);
    emit routeChanged(route);
}

Route* RoutesService::readRoute(const QVariant& id)
{
    QVariantMap select = m_routesRepo->select(id);
    QString typeId = select.value(props::type).toString();
    const RouteType* const type = m_routeTypes.value(typeId);
    if (!type)
    {
        qWarning() << "Unknown route type" << typeId;
        return nullptr;
    }

    Route* route = new Route(type, select, this);
    m_routes.insert(id, route);

    // Read items for route
    for (const QVariant& itemId : m_itemsRepo->selectChildItemsIds(id))
    {
        auto item = this->readItem(itemId);
        if (item)
            route->addItem(item);
    }

    emit routeAdded(route);
    return route;
}

RouteItem* RoutesService::readItem(const QVariant& id)
{
    QVariantMap select = m_itemsRepo->select(id);
    QString typeId = select.value(props::type).toString();
    const RouteItemType* const type = m_itemTypes.value(typeId);
    if (!type)
    {
        qWarning() << "Unknown route item type" << typeId;
        return nullptr;
    }

    // Read current item
    return new RouteItem(type, select);
}

void RoutesService::saveItemImpl(RouteItem* item, const QVariant& parentId,
                                 const QVariantList& itemIds)
{
    // Update stored item
    if (itemIds.contains(item->id))
    {
        m_itemsRepo->update(item, parentId);
    }
    // Insert newbie items
    else
    {
        m_itemsRepo->insert(item, parentId);
    }
}

void RoutesService::restoreItemImpl(RouteItem* item)
{
    m_itemsRepo->read(item);
}

void RoutesService::removeItems(const QVariantList& itemsIds)
{
    for (const QVariant& itemId : itemsIds)
    {
        m_itemsRepo->removeById(itemId);
    }
}