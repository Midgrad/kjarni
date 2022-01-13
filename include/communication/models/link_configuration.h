#ifndef LINK_CONFIGURATION_H
#define LINK_CONFIGURATION_H

#include "link_traits.h"
#include "named.h"

#include <QObject>
#include <QVariantMap>

namespace md::domain
{
class LinkConfiguration : public Named
{
    Q_OBJECT

public:
    // Disabled in order to test mostly immutable link
    //    explicit LinkConfiguration(QString type, const QString& name = QString(),
    //                               QObject* parent = nullptr);
    explicit LinkConfiguration(QVariantMap parameters, const QString& name = QString(),
                               QObject* parent = nullptr);

    QString type() const;
    QVariant parameter(const QString& parameter) const;
    QVariantMap parameters() const;

    bool isConnected() const;

    int bytesReceived() const;
    int bytesSent() const;

public slots:
    void Connect();
    void Disconnect();

signals:
    void connectedChanged(bool connected);
    void bytesReceivedChanged(int bytesReceived);
    void bytesSentChanged(int bytesSent);

private:
    void fillInParameters();
    md::data_source::LinkPtr m_link;
    QString m_type;
    QVariantMap m_parameters;
    bool m_connected;
    int m_bytesReceived;
    int m_bytesSent;
};
} // namespace md::domain

#endif //LINK_CONFIGURATION_H
