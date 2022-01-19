#include "link_transceiver.h"

#include <QDebug>
#include <QThread>
#include <QTimerEvent>

using namespace md::data_source;

namespace
{
constexpr int interval = 100;
} // namespace

LinkTransceiver::LinkTransceiver(const domain::LinkSpecification& linkSpecification,
                                 QObject* parent) :
    ILinkTransceiver(parent),
    m_factory(),
    m_link(createLink(linkSpecification))
{
}

void LinkTransceiver::start()
{
    m_timerId = this->startTimer(::interval);
    this->receiveData();
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

    m_link->checkHandlers();
}

void LinkTransceiver::receiveData()
{
    m_link->asyncReceive([this](const std::string& received_data) {
        emit receivedData(QByteArray::fromStdString(received_data));
        this->receiveData();
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

LinkPtr LinkTransceiver::createLink(const domain::LinkSpecification& specification)
{
    data_source::LinkPtr link;

    if (specification.type() == domain::link_type::udp)
        link = md::data_source::LinkPtr(
            m_factory.create(loodsman::LinkType::udp,
                             specification.parameter(domain::link_parameters::port).toInt()));
    else if (specification.type() == "tcp")
        link = md::data_source::LinkPtr(
            m_factory.create(loodsman::LinkType::tcp,
                             specification.parameter(domain::link_parameters::port).toInt()));
    else
        qWarning() << "Wrong link type";

    Q_ASSERT(link);

    return link;
}