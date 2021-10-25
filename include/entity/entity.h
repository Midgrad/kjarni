#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QUuid>
#include <QVariantMap>

#include "kjarni_traits.h"

namespace md::domain
{
class Entity : public QObject
{
    Q_OBJECT

public:
    Entity(const QUuid& id, const QString& name, const QVariantMap& parameters,
           QObject* parent = nullptr);
    Entity(const QVariantMap& map, QObject* parent = nullptr);
    Entity(const QUuid& id, const QString& name, QObject* parent = nullptr);
    Entity(const QString& name, QObject* parent = nullptr);

    virtual ~Entity();

    QUuid id() const;
    QString name() const;
    const QVariantMap& parameters() const;
    QVariant parameter(const QString& key) const;

    virtual QVariantMap toVariantMap(bool recursive = true) const;
    virtual void fromVariantMap(const QVariantMap& map);

public slots:
    void setName(const QString& name);
    void setParameters(const QVariantMap& parameters);
    void setParameter(const QString& key, const QVariant& value);
    void removeParameters(const QStringList& keys);
    void removeParameter(const QString& key);

signals:
    void nameChanged();
    void parameterChanged(QString key, QVariant value);
    void parametersChanged();

private:
    const QUuid m_id;
    QString m_name;
    QVariantMap m_parameters;
};
} // namespace md::domain

#endif // ENTITY_H