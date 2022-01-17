#ifndef LINK_TRANSCEIVER_H
#define LINK_TRANSCEIVER_H

#include "i_link_transceiver.h"

#include "link_factory.h"
#include "link_traits.h"

namespace md::data_source
{
class LinkTransceiver : public md::domain::ILinkTransceiver
{
    Q_OBJECT

public:
    LinkTransceiver(const data_source::LinkPtr& link, QObject* parent = nullptr);

public slots:
    void start() override;
    void stop() override;

    void send(const QByteArray& data) override;

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    void receiveData();

    int m_timerId = 0;
    data_source::LinkPtr const m_link;
};
} // namespace md::data_source

#endif // LINK_TRANSCEIVER_H