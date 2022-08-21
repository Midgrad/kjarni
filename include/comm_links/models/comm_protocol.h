#ifndef COMM_PROTOCOL_H
#define COMM_PROTOCOL_H

#include <QObject>
#include <QVariantMap>

#include "magic_property.hpp"

namespace md::domain
{
class CommProtocol
{
    Q_GADGET

public:
    CommProtocol(const QString& id, const QString& name);

    utils::ConstProperty<QString> id;
    utils::ConstProperty<QString> name;

    QVariantMap toVariantMap() const;
};
} // namespace md::domain

#endif // COMM_PROTOCOL_H
