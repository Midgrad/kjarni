#ifndef LINK_SPECIFICATION_H
#define LINK_SPECIFICATION_H

#include "communication_traits.h"

#include <QObject>
#include <QVariantMap>

namespace md::domain
{
class LinkSpecification : public QObject
{
    Q_OBJECT

public:
    LinkSpecification() = default;
    LinkSpecification(const QVariantMap& parameters, QObject* parent = nullptr);
    LinkSpecification(const LinkSpecification& another, QObject* parent = nullptr);

    QString type() const;
    QVariant parameter(const QString& parameter) const;
    QVariantMap parameters() const;

private:
    const QString m_type;
    QVariantMap m_parameters;
};
} // namespace md::domain

#endif //LINK_SPECIFICATION_H
