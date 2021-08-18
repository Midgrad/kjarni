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

    auto routeId = kjarni::utils::nameToFilename(name, "json");

    QFile file(m_dir.path() + "/" + routeId);
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);

    QJsonObject modified(data);
    modified.remove(json_params::id);

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
    QStringList routeIds;
    for (const QFileInfo& fileInfo : m_dir.entryList({ "*.json" }, QDir::Files))
    {
        auto routeId = fileInfo.fileName();
        QFile file(m_dir.path() + "/" + routeId);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        QJsonObject routeData = doc.object();
        if (routeData.isEmpty())
            continue;

        routeIds += routeId;
        routeData.insert(json_params::id, routeId);
        m_items[routeId] = routeData;
    }

    for (const QString& routeId : m_items.keys())
    {
        if (routeIds.contains(routeId))
            continue;

        m_items.remove(routeId);
    }
    emit itemsChanged();
}
