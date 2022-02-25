#include "mission_route.h"

#include <QDebug>
#include <QJsonArray>

#include "route_traits.h"

using namespace md::domain;

MissionRoute::MissionRoute(const QString& name, const QVariant& id, QObject* parent) :
    NamedMixin<VisibleMixin<Entity>>(name, true, id, parent)
{
}

QVariantMap MissionRoute::toVariantMap() const
{
    QVariantMap map = NamedMixin<VisibleMixin<Entity>>::toVariantMap();
    return map;
}

void MissionRoute::fromVariantMap(const QVariantMap& map)
{
    NamedMixin<VisibleMixin<Entity>>::fromVariantMap(map);
}

int MissionRoute::count() const
{
    return m_items.count();
}

int MissionRoute::index(MissionRouteItem* item) const
{
    return m_items.indexOf(item);
}

const QList<MissionRouteItem*>& MissionRoute::items() const
{
    return m_items;
}

MissionRouteItem* MissionRoute::item(int index) const
{
    return m_items.value(index, nullptr);
}

void MissionRoute::replaceItem(int index, MissionRouteItem* item)
{
    if (index < 0)
        return;

    if (index > m_items.length() - 1)
        return this->addItem(item);

    m_items.at(index)->deleteLater();
    m_items.replace(index, item);
    emit itemChanged(index, item);
}

void MissionRoute::addItem(MissionRouteItem* item)
{
    if (m_items.contains(item))
        return;

    if (item->thread() != this->thread())
        item->moveToThread(this->thread());

    if (!item->parent())
        item->setParent(this);

    connect(item, &MissionRouteItem::changed, this, [item, this]() {
        emit itemChanged(this->index(item), item);
    });
    connect(item, &MissionRouteItem::goTo, this, [item, this]() {
        emit goTo(this->index(item));
    });

    m_items.append(item);
    emit itemAdded(m_items.count() - 1, item);
}

void MissionRoute::removeItem(MissionRouteItem* item)
{
    int index = m_items.indexOf(item);
    if (index == -1)
        return;

    if (item->parent() == this)
        item->setParent(nullptr);

    disconnect(item, nullptr, this, nullptr);

    m_items.removeOne(item);
    emit itemRemoved(index, item);
    item->deleteLater();
}

void MissionRoute::clear()
{
    for (MissionRouteItem* item : qAsConst(m_items))
    {
        this->removeItem(item);
    }
}
