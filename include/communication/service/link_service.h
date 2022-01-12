#ifndef LINK_SERVICE_H
#define LINK_SERVICE_H

#include <QScopedPointer>

#include "i_json_source.h"
#include "link_factory.h"
#include "link_traits.h"

//TODO: to domain
namespace md::data_source
{
class LinkService
{
public:
    LinkService(const QString& fileName);

    LinkPtrMap links();
    loodsman::LinkFactory* factory();

    void checkHandlers();
    void runHandlers();

private:
    LinkPtrMap createLinks();

    loodsman::LinkFactory m_factory;
    data_source::LinkPtrMap m_links;
    QScopedPointer<IJsonSource> m_source;
};

} // namespace md::data_source

#endif //LINK_SERVICE_H
