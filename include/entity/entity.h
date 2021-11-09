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
    Entity(const QVariant& id = utils::generateId(), const QString& name = QString(),
           const QVariantMap& parameters = QVariantMap(), QObject* parent = nullptr);
    Entity(const QVariantMap& map, QObject* parent = nullptr);

    virtual ~Entity();

    QVariant id() const;
    QString name() const;
    const QVariantMap& parameters() const;
    QVariant parameter(const QString& key) const;

    virtual QVariantMap toVariantMap() const;
    virtual void fromVariantMap(const QVariantMap& map);

public slots:
    void setName(const QString& name);
    void setParameters(const QVariantMap& parameters);
    void setParameter(const QString& key, const QVariant& value);
    void removeParameters(const QStringList& keys);
    void removeParameter(const QString& key);

signals:
    void changed();

private:
    const QVariant m_id;
    QString m_name;
    QVariantMap m_parameters;
};
} // namespace md::domain

#endif // ENTITY_H
