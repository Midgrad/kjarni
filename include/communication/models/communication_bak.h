
#ifndef COMMUNICATION_BAK_H
#define COMMUNICATION_BAK_H

#include "named.h"

#include <QObject>
#include <QVariantMap>

namespace md::domain
{
class Communication : public Named
{
    Q_OBJECT

public:
    Communication(const QString& name = QString(), QObject* parent = nullptr);
    //    Communication(const QString& type, const QVariantMap& map, QObject* parent = nullptr);

    //    Communication& operator=(const Communication& other);

    //    QString protocol() const;
    //    QVariant parameter(const QString& parameter) const;
    //    QVariantMap parameters() const;

    void Activate() const;
    void Deactivate() const;
    bool isActive() const;

    //    int bytesReceived() const;
    //    int bytesSent() const;

private:
    //    ICommunicationProtocol m_protocol;
    //    Communication m_linkSpecification;
    QVariantMap m_parameters;
    bool m_active;
    int m_bytesReceived;
    int m_bytesSent;
};
} // namespace md::domain

#endif //COMMUNICATION_BAK_H
