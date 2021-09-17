#include "json_gateway_files.h"

#include <QDebug>

#include "json_source_file.h"
#include "kjarni_traits.h"

namespace
{
QString filenameFromId(const QVariant& id)
{
    return id.toString() + ".json";
}
} // namespace

using namespace md::data_source;

JsonGatewayFiles::JsonGatewayFiles(const QString& path, QObject* parent) :
    IJsonGateway(parent),
    m_dir(path)
{
    if (!m_dir.exists())
        m_dir.mkpath(".");

    m_watcher.addPath(m_dir.path());
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this,
            &JsonGatewayFiles::itemsChanged);
}

QVariantList JsonGatewayFiles::selectIds() const
{
    QVariantList itemIds;
    for (const QFileInfo& fileInfo : m_dir.entryList({ "*.json" }, QDir::Files))
    {
        // Use fileName as itemId
        itemIds.append(fileInfo.fileName().split(".", QString::SkipEmptyParts).first());
    }
    return itemIds;
}

QList<QJsonObject> JsonGatewayFiles::readAll() const
{
    QList<QJsonObject> items;
    for (const QVariant& itemId : this->selectIds())
    {
        items += this->read(itemId);
    }
    return items;
}

QJsonObject JsonGatewayFiles::read(const QVariant& id) const
{
    JsonSourceFile file(m_dir.path() + "/" + ::filenameFromId(id));

    return file.read().object();
}

void JsonGatewayFiles::save(const QVariant& id, const QJsonObject& data)
{
    JsonSourceFile file(m_dir.path() + "/" + ::filenameFromId(id));

    file.save(QJsonDocument(data));
}

void JsonGatewayFiles::remove(const QVariant& id)
{
    QFile file(m_dir.path() + "/" + ::filenameFromId(id));
    file.remove();
}
