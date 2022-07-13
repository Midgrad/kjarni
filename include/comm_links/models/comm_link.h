#ifndef COMM_LINK_H
#define COMM_LINK_H

#include "comm_link_type.h"
#include "named_mixin.hpp"
#include "parametrised_mixin.hpp"

namespace md::domain
{
class CommLink : public ParametrisedMixin<NamedMixin<Entity>>
{
    Q_OBJECT

public:
    CommLink(const CommLinkType* type, const QString& name,
             const QVariant& id = utils::generateId(), const QString& protocol = QString(),
             const QVariantMap& parameters = QVariantMap(), QObject* parent = nullptr);
    CommLink(const CommLinkType* type, const QVariantMap& map, QObject* parent = nullptr);

    utils::ConstProperty<const CommLinkType*> type;
    utils::Property<QString> protocol;

    QVariantMap toVariantMap() const override;
};
} // namespace md::domain

#endif // COMM_LINK_H
