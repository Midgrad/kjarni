#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QVariantMap>

namespace md::domain
{
class Entity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap parameters READ parameters WRITE setParameters NOTIFY parametersChanged)

public:
    Entity(QObject* parent = nullptr);
    virtual ~Entity();

    const QVariantMap& parameters() const;
    QVariant parameter(const QString& key) const;

public slots:
    void setParameters(const QVariantMap& parameters);
    void setParameter(const QString& key, const QVariant& value);
    void removeParameter(const QString& key);

signals:
    void parameterChanged(QString key, QVariant value);
    void parametersChanged();

private:
    QVariantMap m_parameters;
};
} // namespace md::domain

#endif // ENTITY_H
