#ifndef PLAIN_PARAMETRISED_H
#define PLAIN_PARAMETRISED_H

#include "named_mixin.hpp"

namespace md::domain
{
class PlainParametrised : public NamedMixin<Entity>
{
    Q_OBJECT

public:
    PlainParametrised(const QString& name = QString(), const QVariant& id = utils::generateId(),
                      const QVariantMap& parameters = QVariantMap(), QObject* parent = nullptr);
    PlainParametrised(const QVariantMap& map, QObject* parent = nullptr);

    QVariant parameter(const QString& key) const;
    const QVariantMap& parameters() const;
    bool hasParameter(const QString& key) const;
    void setParameter(const QString& key, const QVariant& value);
    void setParameters(const QVariantMap& parameters);
    void removeParameter(const QString& key);
    void removeParameters(const QStringList& keys);

    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;

protected:
    QVariantMap m_parameters;
};
} // namespace md::domain

#endif // PLAIN_PARAMETRISED_H
