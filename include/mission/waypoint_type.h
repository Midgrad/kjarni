#ifndef WAYPOINT_TYPE_H
#define WAYPOINT_TYPE_H

#include <QVariant>
#include <QVector>

#include "vegur_traits.h"

namespace md::domain
{
class Parameter
{
    Q_GADGET

public:
    enum Type
    {
        Bool,
        Int,
        Real
    };
    Parameter(const QString& name, Type type = Real, const QVariant& defaultValue = 0,
              const QVariant& minValue = QVariant(), const QVariant& maxValue = QVariant(),
              const QVariant& step = 1);
    Parameter(const Parameter& other);

    bool isNull() const;
    QVariant guard(const QVariant& value) const;

    const QString name;
    const Type type;
    const QVariant defaultValue;
    const QVariant minValue;
    const QVariant maxValue;
    const QVariant step;

    Q_ENUM(Type)
};

bool operator==(const Parameter& left, const Parameter& right);

class Waypoint;
class WaypointType
{
    Q_GADGET

public:
    WaypointType(const QString& name, const QVector<Parameter>& parameters);

    Parameter parameter(const QString& name) const;

    void syncParameters(Waypoint* waypoint) const;
    void resetParameter(Waypoint* waypoint, const QString& key) const;

    const QString name;
    const QVector<Parameter> parameters;
};
} // namespace md::domain

#endif // WAYPOINT_TYPE_H
