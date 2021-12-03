#include "mission_route.h"

#include "mission_traits.h"

using namespace md::domain;

MissionRoute::MissionRoute(const MissionType* type, const QVariant& id, QObject* parent) :
    Entity(id, parent),
    m_type(type),
    m_home(new MissionRouteItem(type->homePointType, utils::generateId(), this))
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

void MissionRoute::copyTo(Route* route)
{
    for (int i = 0; i < m_items.count(); ++i)
    {
        MissionRouteItem* src = m_items[i];
        RouteItem* dest;
        if (i < route->count())
        {
            dest = route->item(i);
        }
        else
        {
            dest = new RouteItem(src->type(), src->id());
            route->addItem(dest);
        }
        src->copyTo(dest);
    }
}

void MissionRoute::copyFrom(Route* route)
{
    for (int i = 0; i < route->count(); ++i)
    {
        RouteItem* src = route->item(i);
        MissionRouteItem* dest;
        if (i < m_items.count())
        {
            dest = m_items.value(i);
        }
        else
        {
            dest = new MissionRouteItem(src->type(), src->id());
            m_items.append(dest);
        }
        dest->copyFrom(src);
    }
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
