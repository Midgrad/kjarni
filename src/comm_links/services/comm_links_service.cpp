#include "comm_links_service.h"

#include <QDebug>
#include <QMutexLocker>

#include "comm_links_traits.h"

using namespace md::domain;

CommLinksService::CommLinksService(ICommLinksRepository* commLinksRepo,
                                   ICommLinkTypesRepository* commLinkTypesRepo, QObject* parent) :
    ICommLinksService(parent),
    m_commLinksRepo(commLinksRepo),
    m_commLinkTypesRepo(commLinkTypesRepo),
    m_mutex(QMutex::Recursive)
{
    qRegisterMetaType<CommLink*>("md::domain::CommLink*");

    for (const CommLinkType* type : commLinkTypesRepo->selectCommLinkTypes())
    {
        m_commLinkTypes.insert(type->id(), type);
    }
}

CommLinksService::~CommLinksService()
{
    // TODO: replace with smart ptr or store by value
    qDeleteAll(m_commLinkTypes);
}

CommLink* CommLinksService::commLink(const QVariant& id) const
{
    QMutexLocker locker(&m_mutex);
    return m_commLinks.value(id, nullptr);
}

QVariantList CommLinksService::commLinkIds() const
{
    QMutexLocker locker(&m_mutex);
    return m_commLinks.keys();
}

QList<CommLink*> CommLinksService::commLinks() const
{
    QMutexLocker locker(&m_mutex);
    return m_commLinks.values();
}

QList<const CommLinkType*> CommLinksService::commLinkTypes() const
{
    return m_commLinkTypes.values();
}

const CommLinkType* CommLinksService::commLinkType(const QString& typeId) const
{
    return m_commLinkTypes.value(typeId, nullptr);
}

void CommLinksService::readAll()
{
    QMutexLocker locker(&m_mutex);

    for (const QVariant& commLinkId : m_commLinksRepo->selectCommLinkIds())
    {
        if (!m_commLinks.contains(commLinkId))
        {
            this->readCommLink(commLinkId);
        }
    }
}

void CommLinksService::removeCommLink(CommLink* commLink)
{
    QMutexLocker locker(&m_mutex);

    m_commLinksRepo->remove(commLink);
    m_commLinks.remove(commLink->id);

    emit commLinkRemoved(commLink);
    commLink->deleteLater();
}

void CommLinksService::restoreCommLink(CommLink* commLink)
{
    QMutexLocker locker(&m_mutex);

    m_commLinksRepo->read(commLink);
    emit commLinkChanged(commLink);
}

void CommLinksService::saveCommLink(CommLink* commLink)
{
    QMutexLocker locker(&m_mutex);

    if (m_commLinks.contains(commLink->id))
    {
        m_commLinksRepo->update(commLink);
        emit commLinkChanged(commLink);
    }
    else
    {
        m_commLinksRepo->insert(commLink);
        m_commLinks.insert(commLink->id, commLink);
        commLink->moveToThread(this->thread());
        commLink->setParent(this);
        emit commLinkAdded(commLink);
    }
}

CommLink* CommLinksService::readCommLink(const QVariant& id)
{
    QVariantMap select = m_commLinksRepo->select(id);
    QString typeId = select.value(props::type).toString();
    const CommLinkType* const type = m_commLinkTypes.value(typeId);
    if (!type)
    {
        qWarning() << "Unknown commLink type" << typeId;
        return nullptr;
    }

    CommLink* commLink = new CommLink(type, select, this);

    m_commLinks.insert(id, commLink);
    emit commLinkAdded(commLink);

    return commLink;
}
