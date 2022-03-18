#ifndef MISSION_ROUTE_ITEM_H
#define MISSION_ROUTE_ITEM_H

#include "geodetic.h"
#include "mission_item_type.h"

namespace md::domain
{
class MissionRouteItem : public TypedParametrisedMixin<NamedMixin<Entity>>
{
    Q_OBJECT

public:
    MissionRouteItem(const MissionItemType* type, const QString& name,
                     const QVariant& id = utils::generateId(), const QVariantMap& params = {},
                     const Geodetic& position = Geodetic(), QObject* parent = nullptr);
    MissionRouteItem(const MissionItemType* type, const QVariantMap& map, QObject* parent = nullptr);

    utils::Property<Geodetic> position;
    utils::Property<bool> current;
    utils::Property<bool> reached;

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    const MissionItemType* type() const;

public slots:
    void setType(const MissionItemType* type);
    void syncParameters();

signals:
    void typeChanged();

    void goTo();

private:
    const MissionItemType* m_type;
};
} // namespace md::domain

#endif // MISSION_ROUTE_ITEM_H
