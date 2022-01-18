#ifndef ROUTE_PATTERN_TYPE_H
#define ROUTE_PATTERN_TYPE_H

#include "parametrised.h"

namespace md::domain
{
class RoutePatternType
{
    Q_GADGET

public:
    RoutePatternType(const QString& id, const QString& name, const QString& icon,
                     const QVector<const Parameter*>& parameters);

    QVariantMap toVariantMap() const;

    const Parameter* parameter(const QString& id) const;
    QVariantMap defaultParameters() const;

    const QString id;
    const QString name;
    const QString icon;
    const QMap<QString, const Parameter*> parameters;
};
} // namespace md::domain

#endif // ROUTE_PATTERN_TYPE_H
