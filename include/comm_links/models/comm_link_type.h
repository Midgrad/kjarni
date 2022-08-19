#ifndef COMM_LINK_TYPE_H
#define COMM_LINK_TYPE_H

#include <QObject>
#include <QVariantMap>

#include "typed_parametrised.h"

namespace md::domain
{
class CommLinkType
{
    Q_GADGET

public:
    CommLinkType(const QString& id, const QString& name,
                 const QVector<const ParameterType*>& parameters);
    ~CommLinkType();

    utils::ConstProperty<QString> id;
    utils::ConstProperty<QString> name;

    QVariantMap toVariantMap() const;

    const ParameterType* parameter(const QString& id) const;
    QList<const ParameterType*> parameters() const;
    QStringList parameterKeys() const;

private:
    QMap<QString, const ParameterType*> m_parameters;
};
} // namespace md::domain

#endif // COMM_LINK_TYPE_H
