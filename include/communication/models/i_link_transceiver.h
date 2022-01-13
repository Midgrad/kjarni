#ifndef I_LINK_TRANSCEIVER_H
#define I_LINK_TRANSCEIVER_H

#include <QObject>

namespace md::data_source
{
class ILinkTransceiver : public QObject
{
    Q_OBJECT

public:
    ILinkTransceiver(QObject* parent) : QObject(parent)
    {
    }
    virtual ~ILinkTransceiver() = default;

public slots:
    virtual void start() = 0;
    virtual void stop() = 0;

signals:
    void finished();
};
} // namespace md::domain

#endif // I_LINK_TRANSCEIVER_H
