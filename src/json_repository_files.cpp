#include "json_repository_files.h"

#include <QDebug>
#include <QJsonDocument>

#include "utils.h"

using namespace kjarni::domain;

JsonRepositoryFiles::JsonRepositoryFiles(const QString& path, QObject* parent) :
    IJsonRepository(parent),
    m_dir(path)
{
    if (!m_dir.exists())
        m_dir.mkpath(".");

    m_watcher.addPath(m_dir.path());
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, &JsonRepositoryFiles::scan);

    this->scan();
}

QStringList JsonRepositoryFiles::ids() const
{
    return m_items.keys();
}

QList<QJsonObject> JsonRepositoryFiles::values() const
{
    return m_items.values();
}

QJsonObject JsonRepositoryFiles::value(const QString& id) const
{
    return m_items.value(id);
}

void JsonRepositoryFiles::save(const QJsonObject& data)
{
    QString name = data.value(json_params::name).toString();
    if (name.isEmpty())
        return;

    auto itemId = kjarni::utils::nameToFilename(name, "json");
    QJsonObject modified = data;
    modified[json_params::id] = itemId;

    QFile file(m_dir.path() + "/" + itemId);
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);

    QJsonDocument doc(modified);
    file.write(doc.toJson());
    file.close();
}

void JsonRepositoryFiles::remove(const QString& id)
{
    QFile file(m_dir.path() + "/" + id);
    file.remove();
}

void JsonRepositoryFiles::scan()
{
    QStringList itemIds;
    for (const QFileInfo& fileInfo : m_dir.entryList({ "*.json" }, QDir::Files))
    {
        auto itemId = fileInfo.fileName();
        QFile file(m_dir.path() + "/" + itemId);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        QJsonObject itemData = doc.object();
        if (itemData.isEmpty())
            continue;

        itemIds += itemId;
        m_items[itemId] = itemData;
    }

    for (const QString& itemId : m_items.keys())
    {
        if (itemIds.contains(itemId))
            continue;

        m_items.remove(itemId);
    }
    emit itemsChanged();
}
