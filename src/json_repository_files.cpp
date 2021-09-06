#include "json_repository_files.h"

#include <QDebug>
#include <QJsonDocument>

#include "kjarni_traits.h"
#include "utils.h"

using namespace md::data_source;

JsonRepositoryFiles::JsonRepositoryFiles(const QString& path, QObject* parent) :
    IJsonRepository(parent),
    m_dir(path)
{
    if (!m_dir.exists())
        m_dir.mkpath(".");

    m_watcher.addPath(m_dir.path());
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this,
            &JsonRepositoryFiles::itemsChanged);
}

QStringList JsonRepositoryFiles::selectIds() const
{
    QStringList itemIds;
    for (const QFileInfo& fileInfo : m_dir.entryList({ "*.json" }, QDir::Files))
    {
        // Use fileName as itemId
        itemIds.append(fileInfo.fileName());
    }
    return itemIds;
}

QList<QJsonObject> JsonRepositoryFiles::selectAll() const
{
    QList<QJsonObject> items;
    for (const QString& itemId : this->selectIds())
    {
        items += this->read(itemId);
    }
    return items;
}

QJsonObject JsonRepositoryFiles::read(const QString& id) const
{
    QFile file(m_dir.path() + "/" + id);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QJsonObject();

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    return doc.object();
}

void JsonRepositoryFiles::save(const QJsonObject& data)
{
    QString name = data.value(params::name).toString();
    if (name.isEmpty())
        return;

    auto itemId = md::utils::nameToFilename(name, "json");

    QFile file(m_dir.path() + "/" + itemId);
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);

    QJsonDocument doc(data);
    file.write(doc.toJson());
    file.close();
}

void JsonRepositoryFiles::remove(const QString& id)
{
    QFile file(m_dir.path() + "/" + id);
    file.remove();
}
