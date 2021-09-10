#ifndef ENTITY_H
#define ENTITY_H

#include <QJsonObject>
#include <QObject>
#include <QVariantMap>

#include "kjarni_traits.h"

namespace md::domain
{
class Entity : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant id READ id CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QVariantMap parameters READ parameters WRITE setParameters NOTIFY parametersChanged)

public:
    Entity(const QVariant& id, const QString& name, const QVariantMap& parameters,
           QObject* parent = nullptr);
    Entity(const QJsonObject& object, QObject* parent = nullptr);
    Entity(const QVariant& id, const QString& name, QObject* parent = nullptr);

    virtual ~Entity();

    QVariant id() const;
    QString name() const;
    const QVariantMap& parameters() const;
    QVariant parameter(const QString& key) const;

    QJsonObject toJson() const;

public slots:
    void setName(const QString& name);
    void setParameters(const QVariantMap& parameters);
    void setParameter(const QString& key, const QVariant& value);
    void removeParameter(const QString& key);

signals:
    void nameChanged();
    void parameterChanged(QString key, QVariant value);
    void parametersChanged();

private:
    const QVariant m_id;
    QString m_name;
    QVariantMap m_parameters;
};
} // namespace md::domain

#endif // ENTITY_H
