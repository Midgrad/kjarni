#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "entity.h"
#include "waypoint_type.h"

namespace md::domain
{
class Waypoint : public Entity
{
    Q_OBJECT

public:
    Waypoint(const WaypointType* type, const QString& name, const QVariant& id,
             const QVariantMap& parameters, QObject* parent = nullptr);
    Waypoint(const WaypointType* type, const QString& name,
             const QVariant& id = utils::generateId(), QObject* parent = nullptr);
    Waypoint(const WaypointType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap(bool recursive) const override;
    void fromVariantMap(const QVariantMap& map) override;

    const WaypointType* type() const;

    bool current() const;
    bool reached() const;
    bool confirmed() const;

public slots:
    void setType(const WaypointType* type);

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
    const WaypointType* m_type;
    bool m_current = false;
    bool m_reached = false;
    bool m_confirmed = false;
};
} // namespace md::domain

#endif // WAYPOINT_H
