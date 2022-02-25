#ifndef MISSION_ROUTE_ITEM_H
#define MISSION_ROUTE_ITEM_H

#include "geodetic.h"
#include "named_mixin.hpp"
#include "parameter.h"

namespace md::domain
{
class MissionRouteItem : public NamedMixin<Entity>
{
    Q_OBJECT

public:
    explicit MissionRouteItem(const QString& name, const ParameterList& parameters,
                              const QVariant& id = utils::generateId(), QObject* parent = nullptr);

    utils::ConstProperty<ParameterList> parameters;
    utils::Property<Geodetic> position;
    utils::Property<bool> current;
    utils::Property<bool> reached;

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

signals:
    void goTo();
};
} // namespace md::domain

#endif // MISSION_ROUTE_ITEM_H
