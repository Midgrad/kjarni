#ifndef I_COMM_LINKS_SERVICE_H
#define I_COMM_LINKS_SERVICE_H

#include "comm_link.h"
#include "i_service.h"

namespace md::domain
{
class ICommLinksService
    : public QObject
    , public IService
{
    Q_OBJECT

public:
    ICommLinksService(QObject* parent) : QObject(parent), IService()
    {
    }

    virtual CommLink* commLink(const QVariant& id) const = 0;
    virtual QVariantList commLinkIds() const = 0;
    virtual QList<CommLink*> commLinks() const = 0;
    virtual QList<const CommLinkType*> commLinkTypes() const = 0;
    virtual const CommLinkType* commLinkType(const QString& typeId) const = 0;

public slots:
    virtual void readAll() = 0;
    virtual void removeCommLink(md::domain::CommLink* commLink) = 0;
    virtual void restoreCommLink(md::domain::CommLink* commLink) = 0;
    virtual void saveCommLink(md::domain::CommLink* commLink) = 0;

signals:
    void commLinkAdded(md::domain::CommLink* commLink);
    void commLinkChanged(md::domain::CommLink* commLink);
    void commLinkRemoved(md::domain::CommLink* commLink);
};
} // namespace md::domain

#endif // I_COMM_LINKS_SERVICE_H
