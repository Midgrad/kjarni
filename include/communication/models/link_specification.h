#ifndef LINK_SPECIFICATION_H
#define LINK_SPECIFICATION_H

#include "link_traits.h"

#include <QObject>
#include <QVariantMap>

namespace md::domain
{
class LinkSpecification : public QObject
{
    Q_OBJECT

public:
    explicit LinkSpecification() = default;
    explicit LinkSpecification(QVariantMap parameters, QObject* parent = nullptr);
    explicit LinkSpecification(LinkSpecification& another, QObject* parent = nullptr);

    QString type() const;
    QVariant parameter(const QString& parameter) const;
    QVariantMap parameters() const;

private:
    void fillInParameters();

    const QString m_type;
    QVariantMap m_parameters;
};
} // namespace md::domain

#endif //LINK_SPECIFICATION_H