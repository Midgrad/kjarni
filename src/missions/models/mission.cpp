#include "mission.h"

#include <QDebug>

#include "mission_traits.h"

using namespace md::domain;

Mission::Mission(const MissionType* type, const QString& name, const QVariant& vehicleId,
                 const QVariant& id, QObject* parent) :
    Named(id, name, parent),
    type(type),
    vehicleId(vehicleId),
    home(new RouteItem(type->homePointType, type->homePointType->name)),
    route(nullptr, [this]() {
        emit routeChanged(route);
    })
{
}

Mission::Mission(const MissionType* type, const QVariantMap& map, QObject* parent) :
    Mission(type, map.value(props::name).toString(), map.value(props::vehicle),
            map.value(props::id), parent)
{
}

QVariantMap Mission::toVariantMap() const
{
    QVariantMap map = Named::toVariantMap();

    map.insert(props::type, type.get()->id);
    map.insert(props::vehicle, vehicleId);
    map.insert(props::route, route.get()->id);

    return map;
}

RouteItem* Mission::item(int index)
{
    if (index == 0)
        return home;

    if (index > 0 && route && route()->count() > index - 1) // +1 for home
        return route()->item(index - 1);
    return nullptr;
}

int Mission::count() const
{
    return route ? route.get()->count() + 1 : 1;
}

int Mission::currentItem() const
{
    if (m_currentItem == home)
        return 0;
    if (route)
    {
        int index = route.get()->index(m_currentItem);
        if (index > -1)
            return index + 1;
    }
    return -1;
}

void Mission::setCurrentItem(int index)
{
    if (this->currentItem() == index)
        return;

    if (m_currentItem)
        m_currentItem->current = false;

    m_currentItem = this->item(index);

    if (m_currentItem)
        m_currentItem->current = true;

    emit currentItemChanged(index);
}

void Mission::setReached(int index)
{
    RouteItem* item = this->item(index);
    if (item)
        item->reached = true;

    emit itemReached(index);
}

void Mission::clear()
{
    route()->clear();
}
