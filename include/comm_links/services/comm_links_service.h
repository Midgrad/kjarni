#ifndef COMM_LINKS_SERVICE_H
#define COMM_LINKS_SERVICE_H

#include "i_comm_links_repository.h"
#include "i_comm_links_service.h"

#include <QMutex>

namespace md::domain
{
class CommLinksService : public ICommLinksService
{
    Q_OBJECT

public:
    explicit CommLinksService(ICommLinksRepository* commLinksRepo, QObject* parent = nullptr);

    CommLink* commLink(const QVariant& id) const override;
    QVariantList commLinkIds() const override;
    QList<CommLink*> commLinks() const override;
    QList<const CommLinkType*> commLinkTypes() const override;
    const CommLinkType* commLinkType(const QString& typeId) const override;

public slots:
    void readAll() override;
    void removeCommLink(md::domain::CommLink* commLink) override;
    void restoreCommLink(md::domain::CommLink* commLink) override;
    void saveCommLink(md::domain::CommLink* commLink) override;
    void addCommLinkType(const md::domain::CommLinkType* type) override;
    void removeCommLinkType(const md::domain::CommLinkType* type) override;

private:
    CommLink* readCommLink(const QVariant& id);

    ICommLinksRepository* const m_commLinksRepo;
    QMap<QString, const CommLinkType*> m_commLinkTypes;
    QMap<QVariant, CommLink*> m_commLinks;

    mutable QMutex m_mutex;
};
} // namespace md::domain

#endif // COMM_LINKS_SERVICE_H
