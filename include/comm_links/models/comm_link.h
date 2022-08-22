#ifndef COMM_LINK_H
#define COMM_LINK_H

#include "comm_link_type.h"
#include "named_mixin.hpp"

namespace md::domain
{
class CommLink : public TypedParametrisedMixin<NamedMixin<Entity>>
{
    Q_OBJECT

public:
    CommLink(const CommLinkType* type, const QString& name,
             const QVariant& id = utils::generateId(), const QString& protocol = QString(),
             const QVariantMap& params = QVariantMap(), QObject* parent = nullptr);
    CommLink(const CommLinkType* type, const QVariantMap& map, QObject* parent = nullptr);

    utils::ConstProperty<const CommLinkType*> type;
    utils::Property<QString> protocol;
    utils::Property<bool> autoconnect;

    utils::Property<bool> connected;
    utils::Property<bool> online;

    QVariantMap toVariantMap() const override;

public slots:
    void setType(const domain::CommLinkType* type);
    void resertToDefaultParameters();

signals:
    void connectDisconnectLink(bool connect);

private:
    const CommLinkType* m_type;
};
} // namespace md::domain

#endif // COMM_LINK_H
