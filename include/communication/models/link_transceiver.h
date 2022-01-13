#ifndef LINK_TRANSCEIVER_H
#define LINK_TRANSCEIVER_H

#include "i_link_transceiver.h"

#include "link_factory.h"
#include "link_traits.h"

namespace md::data_source
{
class LinkTransceiver : public ILinkTransceiver
{
    Q_OBJECT

public:
    LinkTransceiver(const data_source::LinkPtr& link, loodsman::LinkFactory* linkFactory,
                    QObject* parent = nullptr);

public slots:
    void start() override;
    void stop() override;

    void send(const QByteArray& data);

protected:
    void timerEvent(QTimerEvent* event) override;

signals:
    void receivedData(QByteArray data);

private:
    void receiveData();

    int m_timerId = 0;
    data_source::LinkPtr const m_link;
    loodsman::LinkFactory* m_factory;
};
} // namespace md::data_source

#endif // LINK_TRANSCEIVER_H
