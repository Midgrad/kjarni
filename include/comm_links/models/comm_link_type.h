#ifndef COMM_LINK_TYPE_H
#define COMM_LINK_TYPE_H

#include <QObject>
#include <QVariantMap>

namespace md::domain
{
class CommLinkType
{
    Q_GADGET

public:
    CommLinkType(const QString& id, const QString& name);

    QVariantMap toVariantMap() const;

    const QString id;
    const QString name;
};
} // namespace md::domain

#endif // COMM_LINK_TYPE_H
