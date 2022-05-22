#include "mission_route.h"

#include <QDebug>
#include <QJsonArray>

#include "mission_traits.h"

using namespace md::domain;

MissionRoute::MissionRoute(const QString& name, const QVariant& id, QObject* parent) :
    NamedMixin<Entity>(name, id, parent)
{
}

MissionRoute::MissionRoute(const QVariantMap& map, QObject* parent) :
    MissionRoute(map.value(props::name).toString(), map.value(props::id), parent)
{
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
    return m_currentIndex;
}

MissionRouteItem* MissionRoute::firstItem() const
{
    return m_items.isEmpty() ? nullptr : m_items.first();
}

MissionRouteItem* MissionRoute::lastItem() const
{
    return m_items.isEmpty() ? nullptr : m_items.last();
}

MissionRouteItem* MissionRoute::currentItem() const
{
    return this->item(m_currentIndex);
}

const QList<MissionRouteItem*>& MissionRoute::items() const
{
    return m_items;
}

MissionRouteItem* MissionRoute::item(int index) const
{
    return m_items.value(index, nullptr);
}

void MissionRoute::setItems(const QList<MissionRouteItem*>& items)
{
    // Remove old items (std::remove_if does not emit signals)
    for (MissionRouteItem* item : qAsConst(m_items))
    {
        // Skip item if we have it in new list
        if (items.contains(item))
            continue;

        this->removeItem(item);
    }

    // Add new items to the end
    for (MissionRouteItem* item : items)
    {
        this->addItem(item);
    }
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
        emit changed();
    });
    connect(item, &MissionRouteItem::goTo, this, [item, this]() {
        emit goTo(this->index(item));
    });

    m_items.append(item);
    emit itemAdded(m_items.count() - 1, item);
    emit changed();
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
    emit changed();
    item->deleteLater();
}

void MissionRoute::clear()
{
    for (MissionRouteItem* item : qAsConst(m_items))
    {
        this->removeItem(item);
    }
}

void MissionRoute::setReached(int index)
{
    if (index < 0 || index >= m_items.count())
        return;

    m_items.at(index)->reached.set(true);
}

void MissionRoute::setCurrent(int index)
{
    if (m_currentIndex == index)
        return;

    if (m_currentIndex > -1 && m_currentIndex < m_items.count())
        m_items.at(m_currentIndex)->current.set(false);

    m_currentIndex = index;

    if (index > -1 && index < m_items.count())
        m_items.at(index)->current.set(true);

    emit currentChanged(index);
}
