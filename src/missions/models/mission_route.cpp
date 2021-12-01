#include "mission_route.h"

#include "mission_traits.h"

using namespace md::domain;

MissionRoute::MissionRoute(const MissionType* type, const QVariant& id, QObject* parent) :
    Entity(id, parent),
    m_type(type),
    m_home(new MissionRouteItem(type->homePointType, utils::generateId(), this))
{
}

MissionRoute::MissionRoute(const MissionType* type, const QVariantMap& map, QObject* parent) :
    Entity(map, parent),
    m_type(type),
    m_home(new MissionRouteItem(type->homePointType, map.value(props::home), this))
{
}

int MissionRoute::count() const
{
    return m_items.count() + 1; // One for home
}

MissionRouteItem* MissionRoute::homeItem() const
{
    return m_home;
}

QList<MissionRouteItem*> MissionRoute::items() const
{
    QList<MissionRouteItem*> items;

    items.append(m_home);
    for (MissionRouteItem* item : m_items)
    {
        items.append(item);
    }

    return m_items;
}

MissionRouteItem* MissionRoute::item(int index) const
{
    return m_items.value(index, nullptr);
}

int MissionRoute::currentItem() const
{
    return m_currentItem;
}

Route* MissionRoute::toPlainRoute() const
{
    // FIXME: temp crutch

    Route* route = new Route(m_type->routeType, "TEST", this->id());
    for (MissionRouteItem* item : m_items)
    {
        route->addItem(item);
    }
    return route;
}

void MissionRoute::addItem(MissionRouteItem* item)
{
    m_items.append(item);

    emit itemsChanged();
}

void MissionRoute::removeItem(MissionRouteItem* item)
{
    if (m_items.removeOne(item))
    {
        emit itemsChanged();
        item->deleteLater();
    }
}

void MissionRoute::clear()
{
    qDeleteAll(m_items);
    m_items.clear();
}

void MissionRoute::setCurrentItem(int currentItem)
{
    if (m_currentItem == currentItem)
        return;

    m_currentItem = currentItem;
    emit currentItemChanged(m_currentItem);
}
