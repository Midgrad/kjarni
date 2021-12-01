#include "mission_route.h"

#include "mission_traits.h"

using namespace md::domain;

MissionRoute::MissionRoute(Route* underlyingRoute, QObject* parent) :
    Entity(underlyingRoute->id(), parent),
    m_underlyingRoute(underlyingRoute)
{
    for (RouteItem* item : underlyingRoute->items())
    {
        m_items.append(new MissionRouteItem(item, this));
    }

    //    connect(underlyingRoute, &Route::itemChanged, this, &MissionRoute::itemsChanged);
    //    connect(underlyingRoute, &Route::itemAdded, this, [this](int index, RouteItem* item) {
    //        m_items.append(new MissionRouteItem(item, this));
    //        emit itemsChanged();
    //    });
    //    connect(underlyingRoute, &Route::itemRemoved, this, [this](int index, RouteItem* item) {
    //        m_items.erase(std::remove_if(m_items.begin(), m_items.end(),
    //                                     [item](MissionRouteItem* missionItem) {
    //                                         return missionItem->underlyingItem() == item;
    //                                     }),
    //                      m_items.end());

    //        emit itemsChanged();
    //    });
}

Route* MissionRoute::underlyingRoute() const
{
    return m_underlyingRoute;
}

int MissionRoute::count() const
{
    return m_items.count();
}

QList<MissionRouteItem*> MissionRoute::items() const
{
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

void MissionRoute::addNewItem(MissionRouteItem* item)
{
    m_items.append(item);
    m_underlyingRoute->addItem(item->underlyingItem());

    emit itemsChanged();
}

void MissionRoute::setCurrentItem(int currentItem)
{
    if (m_currentItem == currentItem)
        return;

    m_currentItem = currentItem;
    emit currentItemChanged(m_currentItem);
}

void MissionRoute::clear()
{
    m_underlyingRoute->clear();
}
