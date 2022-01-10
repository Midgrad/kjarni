
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "named.h"

#include <QObject>
#include <QVariantMap>

namespace md::domain
{
class Communication : public Named
{
    Q_OBJECT

public:
    Communication(const QString& type, const QString& name = QString(),
          const QVariant& id = utils::generateId(), QObject* parent = nullptr);
    Communication(const QString& type, const QVariantMap& map, QObject* parent = nullptr);

    Communication& operator=(const Communication& other);

    QString type() const;
    QString protocol() const;
    QVariant parameter(const QString& parameter) const;
    QVariantMap parameters() const;

    bool isConnected() const;

    int bytesReceived() const;
    int bytesSent() const;


private:
    QString m_type;
    QString m_protocol;
    QVariantMap m_parameters;
    bool m_connected;
    int m_bytesReceived;
    int m_bytesSent;

};
} // namespace md::domain

#endif //COMMUNICATION_H
