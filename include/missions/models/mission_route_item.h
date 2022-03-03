#ifndef MISSION_ROUTE_ITEM_H
#define MISSION_ROUTE_ITEM_H

#include "geodetic.h"
#include "named_mixin.hpp"
#include "parameter.h"

namespace md::domain
{
class MissionRouteItemType
{
public:
    MissionRouteItemType(const QString& id, const QString& name);

    utils::ConstProperty<QString> id;
    utils::ConstProperty<QString> name;

    virtual ParameterMap createParameters() = 0;
};

class MissionRouteItem : public NamedMixin<Entity>
{
    Q_OBJECT

public:
    explicit MissionRouteItem(MissionRouteItemType* type, const QVariant& id = utils::generateId(),
                              QObject* parent = nullptr);

    utils::Property<Geodetic> position;
    utils::Property<bool> current;
    utils::Property<bool> reached;

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    MissionRouteItemType* type() const;
    ParameterList parameters() const;
    Parameter* parameter(const QString& id) const;

    void setType(MissionRouteItemType* type);
    void syncParameters();

signals:
    void goTo();

private:
    MissionRouteItemType* m_type;
    ParameterMap m_parameters;
};
} // namespace md::domain

#endif // MISSION_ROUTE_ITEM_H
