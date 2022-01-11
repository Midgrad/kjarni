#ifndef NAMED_H
#define NAMED_H

#include "entity.h"

namespace md::domain
{
class Named : public Entity
{
    Q_OBJECT

public:
    explicit Named(const QString& name = QString(), const QVariant& id = utils::generateId(),
          QObject* parent = nullptr);

    utils::Property<QString> name;

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;
};
} // namespace md::domain

#endif // NAMED_H
