#ifndef COMM_LINKS_SERVICE_H
#define COMM_LINKS_SERVICE_H

#include "i_comm_link_types_repository.h"
#include "i_comm_links_repository.h"
#include "i_comm_links_service.h"

#include <QMutex>

namespace md::domain
{
class CommLinksService : public ICommLinksService
{
    Q_OBJECT

public:
    explicit CommLinksService(ICommLinksRepository* commLinksRepo,
                              ICommLinkTypesRepository* commLinkTypesRepo,
                              QObject* parent = nullptr);
    ~CommLinksService() override;

    CommLink* commLink(const QVariant& id) const override;
    QVariantList commLinkIds() const override;
    QList<CommLink*> commLinks() const override;
    QList<const CommLinkType*> commLinkTypes() const override;
    const CommLinkType* commLinkType(const QString& typeId) const override;

    QList<CommProtocol> protocols() const override;

public slots:
    void readAll() override;
    void removeCommLink(md::domain::CommLink* commLink) override;
    void restoreCommLink(md::domain::CommLink* commLink) override;
    void saveCommLink(md::domain::CommLink* commLink) override;

    void addProtocol(const CommProtocol& protocol) override;
    void removeProtocol(const CommProtocol& protocol) override;

private:
    CommLink* readCommLink(const QVariant& id);

    ICommLinksRepository* const m_commLinksRepo;
    ICommLinkTypesRepository* m_commLinkTypesRepo;
    QMap<QString, const CommLinkType*> m_commLinkTypes;
    QMap<QVariant, CommLink*> m_commLinks;

    QMap<QString, CommProtocol> m_protocols;

    mutable QMutex m_mutex;
};
} // namespace md::domain

#endif // COMM_LINKS_SERVICE_H
