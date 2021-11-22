#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QVariantMap>

#include "kjarni_traits.h"
#include "utils.h"

namespace md::domain
{
class Entity : public QObject
{
    Q_OBJECT

public:
    Entity(const QVariant& id = utils::generateId(), QObject* parent = nullptr);
    Entity(const QVariantMap& map, QObject* parent = nullptr);
    virtual ~Entity();

    QVariant id() const;

    virtual QVariantMap toVariantMap() const;
    virtual void fromVariantMap(const QVariantMap& map);

signals:
    void changed();

private:
    const QVariant m_id;
};
} // namespace md::domain

#endif // ENTITY_H
