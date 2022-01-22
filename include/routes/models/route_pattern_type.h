#ifndef ROUTE_PATTERN_TYPE_H
#define ROUTE_PATTERN_TYPE_H

#include "typed_parametrised.h"

namespace md::domain
{
class RoutePatternType
{
    Q_GADGET

public:
    RoutePatternType(const QString& id, const QString& name, const QString& icon,
                     const QVector<const ParameterType*>& parameters);

    QVariantMap toVariantMap() const;

    const ParameterType* parameter(const QString& id) const;
    QVariantMap defaultParameters() const;

    const QString id;
    const QString name;
    const QString icon;
    const QMap<QString, const ParameterType*> parameters;
};
} // namespace md::domain

#endif // ROUTE_PATTERN_TYPE_H
