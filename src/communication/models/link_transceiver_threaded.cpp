#include "link_transceiver_threaded.h"
#include "link_transceiver.h"

#include "mavlink_transceiver.h"

#include <QDebug>

namespace
{
constexpr int timeout = 500;
constexpr char threadName[] = "link_transceiver";
} // namespace

using namespace md::data_source;

LinkTransceiverThreaded::LinkTransceiverThreaded(ILinkTransceiver* worker, QObject* parent) :
    ILinkTransceiver(parent),
    m_worker(worker),
    m_thread(new QThread(this))
{
    Q_ASSERT(worker);

    m_thread->setObjectName(::threadName);
    worker->moveToThread(m_thread);

    QObject::connect(m_worker, &ILinkTransceiver::finished, m_thread, &QThread::quit);
    QObject::connect(m_worker, &ILinkTransceiver::finished, m_worker, &QObject::deleteLater);
    QObject::connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);

    //    QObject::connect(m_worker, &ILinkTransceiver::finished, this, &ILinkTransceiver::finished);
}

LinkTransceiverThreaded::~LinkTransceiverThreaded()
{
    m_thread->quit();

    if (!m_thread->wait(::timeout))
    {
        qCritical() << "Thread" << m_thread->objectName() << "is blocked!";
        qCritical() << "Forcing to terminate...";
        m_thread->terminate();
    }
}

void LinkTransceiverThreaded::start()
{
    m_thread->start();

    QMetaObject::invokeMethod(m_worker, "start", Qt::QueuedConnection);
}

void LinkTransceiverThreaded::stop()
{
    QMetaObject::invokeMethod(m_worker, "stop", Qt::QueuedConnection);
}
