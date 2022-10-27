#ifndef ENTITY_H
#define ENTITY_H

#include "export.h"

#include <QObject>
#include <QVariantMap>

#include "kjarni_traits.h"
#include "magic_property.hpp"
#include "utils.h"

namespace md::domain
{
class KJARNI_EXPORT Entity : public QObject
{
    Q_OBJECT

public:
    explicit Entity(const QVariant& id = utils::generateId(), QObject* parent = nullptr);
    Entity(const QVariantMap& map, QObject* parent = nullptr);
    ~Entity() override;

    utils::ConstProperty<QVariant> id;

    virtual QVariantMap toVariantMap() const;
    virtual void fromVariantMap(const QVariantMap& map);

signals:
    void changed();
};
} // namespace md::domain

#endif // ENTITY_H
