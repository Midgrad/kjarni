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

bool MissionRoute::isEmpty() const
{
    return m_items.isEmpty();
}

int MissionRoute::count() const
{
    return m_items.count();
}

int MissionRoute::index(MissionRouteItem* item) const
{
    return m_items.indexOf(item);
}

int MissionRoute::currentIndex() const
{
    return m_currentItemIndex;
}

const QList<MissionRouteItem*>& MissionRoute::items() const
{
    return m_items;
}

MissionRouteItem* MissionRoute::first() const
{
    return m_items.isEmpty() ? nullptr : m_items.first();
}

MissionRouteItem* MissionRoute::item(int index) const
{
    if (index >= 0 && index < m_items.count())
        return m_items.value(index, nullptr);

    return nullptr;
}

MissionRouteItem* MissionRoute::currentItem() const
{
    return this->item(m_currentItemIndex);
}

void MissionRoute::replaceItem(int index, MissionRouteItem* item)
{
    if (index < 0)
        return;

    if (index > m_items.length() - 1)
        return this->addItem(item);

    m_items.at(index)->deleteLater();
    m_items.replace(index, item);

    if (index == m_currentItemIndex)
        item->current.set(true);

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

void MissionRoute::reachItem(int index)
{
    if (index < 0 || index >= m_items.count())
        return;

    m_items.at(index)->reached.set(true);
    emit itemReached(index);
}

void MissionRoute::setCurrent(int index)
{
    if (m_currentItemIndex == index)
        return;

    if (m_currentItemIndex >= 0 && m_currentItemIndex < m_items.count())
    {
        m_items.at(m_currentItemIndex)->current.set(true);
    }

    m_currentItemIndex = index;

    if (index >= 0 && index < m_items.count())
    {
        m_items.at(index)->current.set(true);
    }

    emit currentItemChanged(index);
}
