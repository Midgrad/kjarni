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
    // TODO: replace with bool flags
    enum State
    {
        Unknown,
        Current,
        Normal,
        Reached,
        Unconfirmed
    };

    Waypoint(const WaypointType* type, const QString& name, const QVariant& id,
             const QVariantMap& parameters, QObject* parent = nullptr);
    Waypoint(const WaypointType* type, const QString& name,
             const QVariant& id = utils::generateId(), QObject* parent = nullptr);
    Waypoint(const WaypointType* type, const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap(bool recursive) const override;
    void fromVariantMap(const QVariantMap& map) override;

    const WaypointType* type() const;
    State state() const;

public slots:
    void setType(const WaypointType* type);
    void setState(State state);
    void setAndCheckParameter(const QString& key, const QVariant& value);
    void resetParameter(const QString& key);
    void resetParameters();
    void syncParameters();

signals:
    void typeChanged();
    void stateChanged();

private:
    const WaypointType* m_type;
    State m_state;

    Q_ENUM(State);
};
} // namespace md::domain

#endif // WAYPOINT_H
