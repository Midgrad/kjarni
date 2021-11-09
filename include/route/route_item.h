#ifndef ROUTE_ITEM_H
#define ROUTE_ITEM_H

#include "entity.h"
#include "route_item_type.h"

namespace md::domain
{
class RouteItem : public Entity
{
    Q_OBJECT

public:
    RouteItem(const RouteItemType* type, const QString& name, const QVariant& id,
              const QVariantMap& parameters, QObject* parent = nullptr);
    RouteItem(const RouteItemType* type, const QString& name,
              const QVariant& id = utils::generateId(), QObject* parent = nullptr);
    RouteItem(const RouteItemType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    const RouteItemType* type() const;

    bool current() const;
    bool reached() const;
    bool confirmed() const;

public slots:
    void setType(const RouteItemType* type);

    void setCurrent(bool current);
    void setReached(bool reached);
    void setConfirmed(bool confirmed);

    void setAndCheckParameter(const QString& key, const QVariant& value);
    void resetParameter(const QString& key);
    void resetParameters();
    void syncParameters();

signals:
    void typeChanged();

private:
    const RouteItemType* m_type;
    bool m_current = false;
    bool m_reached = false;
    bool m_confirmed = false;
};
} // namespace md::domain

#endif // ROUTE_ITEM_H
