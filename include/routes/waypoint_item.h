#ifndef WAYPOINT_ITEM_H
#define WAYPOINT_ITEM_H

#include "entity.h"
#include "waypoint_item_type.h"

namespace md::domain
{
class WaypointItem : public Parametrised
{
    Q_OBJECT

public:
    WaypointItem(const WaypointItemType* type, const QVariant& id = utils::generateId(),
                 QObject* parent = nullptr);
    WaypointItem(const WaypointItemType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    virtual const WaypointItemType* type() const;

public slots:
    void setType(const WaypointItemType* type);

    void setAndCheckParameter(const QString& paramId, const QVariant& value);
    void resetParameter(const QString& paramId);
    void resetParameters();
    void syncParameters();

signals:
    void typeChanged();

private:
    const WaypointItemType* m_type;
};
} // namespace md::domain

#endif // WAYPOINT_ITEM_H
