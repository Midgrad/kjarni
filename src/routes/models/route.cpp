#include "route.h"

#include <QDebug>
#include <QJsonArray>

#include "route_traits.h"

using namespace md::domain;

Route::Route(const RouteType* type, const QString& name, const QVariant& id, QObject* parent) :
    Named(id, name, parent),
    block(QString(), std::bind(&Entity::changed, this)),
    type(type)
{
    Q_ASSERT(type);
}

Route::Route(const RouteType* type, const QVariantMap& map, QObject* parent) :
    Route(type, map.value(props::name).toString(), map.value(props::id), parent)
{
}

QVariantMap Route::toVariantMap() const
{
    QVariantMap map = Named::toVariantMap();

    map.insert(props::type, this->type()->id);
    map.insert(props::block, this->block());

    return map;
}

void Route::fromVariantMap(const QVariantMap& map)
{
    Named::fromVariantMap(map);
}

int Route::count() const
{
    return m_items.count();
}

int Route::index(RouteItem* item) const
{
    return m_items.indexOf(item);
}

const QList<RouteItem*>& Route::items() const
{
    return m_items;
}

RouteItem* Route::item(int index) const
{
    return m_items.value(index, nullptr);
}

void Route::setItems(const QList<RouteItem*>& items)
{
    // Remove old items (std::remove_if does not emit signals)
    for (RouteItem* item : qAsConst(m_items))
    {
        // Skip item if we have it in new list
        if (items.contains(item))
            continue;

        this->removeItem(item);
    }

    // Add new items to the end
    for (RouteItem* item : items)
    {
        this->addItem(item);
    }
}

void Route::addItem(RouteItem* item)
{
    if (m_items.contains(item))
        return;

    if (item->thread() != this->thread())
        item->moveToThread(this->thread());

    if (!item->parent())
        item->setParent(this);

    connect(item, &RouteItem::changed, this, [item, this]() {
        emit itemChanged(this->index(item), item);
    });
    connect(item, &RouteItem::goTo, this, [item, this]() {
        emit goTo(this->index(item));
    });

    m_items.append(item);
    emit itemAdded(m_items.count() - 1, item);
}

void Route::removeItem(RouteItem* item)
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

void Route::clear()
{
    for (RouteItem* item : qAsConst(m_items))
    {
        this->removeItem(item);
    }
}
