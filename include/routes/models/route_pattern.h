#ifndef ROUTE_PATTERN_H
#define ROUTE_PATTERN_H

#include "parametrised.h"

namespace md::domain
{
class RoutePattern
{
    Q_GADGET

public:
    RoutePattern(const QString& id, const QString& name,
                 const QVector<const Parameter*>& parameters);

    QVariantMap toVariantMap() const;

    const Parameter* parameter(const QString& id) const;
    QVariantMap defaultParameters() const;

    const QString id;
    const QString name;
    const QMap<QString, const Parameter*> parameters;
};
} // namespace md::domain

#endif // ROUTE_PATTERN_H
