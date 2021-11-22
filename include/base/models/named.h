#ifndef NAMED_H
#define NAMED_H

#include "entity.h"

namespace md::domain
{
class Named : public Entity
{
    Q_OBJECT

public:
    Named(const QVariant& id = utils::generateId(), const QString& name = QString(),
          QObject* parent = nullptr);
    Named(const QVariantMap& map, QObject* parent = nullptr);

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

    QString name() const;

public slots:
    void setName(const QString& name);

private:
    QString m_name;
};
} // namespace md::domain

#endif // NAMED_H
