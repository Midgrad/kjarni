#ifndef LINK_TRANSCEIVER_THREADED_H
#define LINK_TRANSCEIVER_THREADED_H

#include "i_link_transceiver.h"

#include <QThread>

namespace md::data_source
{
class LinkTransceiverThreaded : public ILinkTransceiver
{
    Q_OBJECT

public:
    LinkTransceiverThreaded(ILinkTransceiver* worker, QObject* parent = nullptr);
    ~LinkTransceiverThreaded() override;

public slots:
    void start() override;
    void stop() override;

    void send(const QByteArray& data) override;

signals:
    void resendData(QByteArray data);

private:
    ILinkTransceiver* const m_worker;
    QThread* const m_thread;
};
} // namespace md::data_source

#endif // LINK_TRANSCEIVER_THREADED_H
