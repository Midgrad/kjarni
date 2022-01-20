#ifndef LINK_SPECIFICATION_H
#define LINK_SPECIFICATION_H

#include <QVariantMap>

#include "communication_traits.h"

namespace md::domain
{
class LinkSpecification
{
public:
    LinkSpecification() = default;
    LinkSpecification(const QVariantMap& parameters);
    LinkSpecification(const LinkSpecification& another, QObject* parent = nullptr);

    QString type() const;
    QVariant parameter(const QString& parameter) const;
    QVariantMap parameters() const;

private:
    const QString m_type;
    const QVariantMap m_parameters;
};
} // namespace md::domain

#endif //LINK_SPECIFICATION_H
