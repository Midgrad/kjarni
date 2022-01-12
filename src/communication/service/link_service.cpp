#include "link_service.h"

#include <QDebug>

#include <QJsonArray>
#include <QJsonObject>

#include "json_source_file.h"
#include "link_factory.h"

using namespace md::data_source;

namespace
{
constexpr char type[] = "type";
constexpr char name[] = "name";
constexpr char localPort[] = "local_port";

} // namespace

LinkService::LinkService(const QString& fileName) :
    m_source(new JsonSourceFile(fileName)),
    m_factory()
{
    m_links = createLinks();
}

LinkPtrMap LinkService::createLinks()
{
    QJsonDocument document = m_source->read();

    QMap<QString, LinkPtr> links;
    for (const QJsonValue& value : document.array())
    {
        QJsonObject linkConfig = value.toObject();

        QString type = (linkConfig.value(::type).toString());
        LinkPtr link;

        if (type == "udp")
            link = LinkPtr(
                m_factory.create(loodsman::LinkType::udp, linkConfig.value(::localPort).toInt()));
        else if (type == "tcp")
            link = LinkPtr(
                m_factory.create(loodsman::LinkType::tcp, linkConfig.value(::localPort).toInt()));
        else
            qWarning() << "Wrong link type in " << linkConfig.value(::name).toString();

        if (link)
            links[linkConfig.value(::name).toString()] = link;
    }

    return links;
}

LinkPtrMap LinkService::links()
{
    return m_links;
}

loodsman::LinkFactory* LinkService::factory()
{
    return &m_factory;
}

void LinkService::checkHandlers()
{
    m_factory.checkHandlers();
}

void LinkService::runHandlers()
{
    m_factory.runHandlers();
}