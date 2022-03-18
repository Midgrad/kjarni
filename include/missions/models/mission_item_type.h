#ifndef MISSION_ITEM_TYPE_H
#define MISSION_ITEM_TYPE_H

#include "typed_parametrised.h"

namespace md::domain
{
enum class Positioned
{
    No,
    Required,
    Optional
};

class MissionRouteItem;
class MissionItemType
{
    Q_GADGET

public:
    MissionItemType(const QString& id, const QString& name, const QString& shortName,
                    Positioned positioned, const QVector<const ParameterType*>& parameters = {});

    QVariantMap toVariantMap() const;

    const ParameterType* parameter(const QString& id) const;
    QVariantMap defaultParameters() const;

    const QString id;
    const QString name;
    const QString shortName;
    const Positioned positioned;
    const QMap<QString, const ParameterType*> parameters;
};
} // namespace md::domain

#endif // MISSION_ITEM_TYPE_H
