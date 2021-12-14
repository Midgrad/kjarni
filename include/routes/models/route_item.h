#ifndef ROUTE_ITEM_H
#define ROUTE_ITEM_H

#include "entity.h"
#include "geodetic.h"
#include "route_item_type.h"

namespace md::domain
{
class RouteItem : public Parametrised
{
    Q_OBJECT

public:
    RouteItem(const RouteItemType* type, const QVariant& id = utils::generateId(),
              const QString& name = QString(), QVariantMap params = {},
              const Geodetic& position = Geodetic(), const QVariantMap& calcData = {},
              bool current = false, bool reached = false, QObject* parent = nullptr);
    RouteItem(const RouteItemType* type, const QVariantMap& map, QObject* parent = nullptr);

    utils::Property<Geodetic> position;
    utils::Property<QVariantMap> calcData;
    utils::Property<bool> current;
    utils::Property<bool> reached;

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    const RouteItemType* type() const;

public slots:
    void setType(const RouteItemType* type);

    void setAndCheckParameter(const QString& paramId, const QVariant& value);
    void resetParameter(const QString& paramId);
    void resetParameters();
    void syncParameters();

signals:
    void typeChanged();

private:
    const RouteItemType* m_type;
};
} // namespace md::domain

#endif // ROUTE_ITEM_H