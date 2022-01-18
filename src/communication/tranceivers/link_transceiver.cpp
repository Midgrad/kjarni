#include "link_transceiver.h"

#include <QDebug>
#include <QThread>
#include <QTimerEvent>

using namespace md::data_source;

namespace
{
constexpr int interval = 100;
} // namespace

LinkTransceiver::LinkTransceiver(const data_source::LinkPtr& link, QObject* parent) :
    ILinkTransceiver(parent),
    m_link(link)

{
}

void LinkTransceiver::start()
{
    m_timerId = this->startTimer(::interval);
    receiveData();
}

void LinkTransceiver::stop()
{
    if (m_timerId)
    {
        this->killTimer(m_timerId);
        m_timerId = 0;
    }

    emit finished();
}

void LinkTransceiver::timerEvent(QTimerEvent* event)
{
    if (event->timerId() != m_timerId)
        return QObject::timerEvent(event);

    // TODO: check if signals and slots works with runHandlers()
    m_link->checkHandlers();
}

void LinkTransceiver::receiveData()
{
    m_link->asyncReceive([this](const std::string& received_data) {
        emit receivedData(QByteArray::fromStdString(received_data));
        receiveData();
    });
}

void LinkTransceiver::send(const QByteArray& data)
{
    m_link->asyncSend(data.toStdString(), [](std::size_t size) {});
}
void LinkTransceiver::openLink()
{
    m_link->open();
}

void LinkTransceiver::closeLink()
{
    m_link->close();
}
