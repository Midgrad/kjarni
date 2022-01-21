#ifndef LINK_TRANSCEIVER_H
#define LINK_TRANSCEIVER_H

#include "i_link_transceiver.h"

#include "communication_traits.h"
#include "link_factory.h"
#include "link_ptr.h"
#include "link_specification.h"

namespace md::data_source
{
class LinkTransceiver : public ILinkTransceiver
{
    Q_OBJECT

public:
    LinkTransceiver(const domain::LinkSpecification& linkSpecification, QObject* parent = nullptr);

public slots:
    void start() override;
    void stop() override;

    void send(const QByteArray& data) override;

    void openLink();
    void closeLink();

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    void receiveData();

    LinkPtr createLink(const domain::LinkSpecification& specification);

    int m_timerId = 0;
    loodsman::LinkFactory m_factory;
    LinkPtr const m_link;
};
} // namespace md::data_source

#endif // LINK_TRANSCEIVER_H
