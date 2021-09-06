#ifndef JSON_REPOSITORY_FILES_H
#define JSON_REPOSITORY_FILES_H

#include "i_json_repository.h"

#include <QDir>
#include <QFileSystemWatcher>
#include <QMap>

namespace md::data_source
{
class JsonRepositoryFiles : public IJsonRepository
{
    Q_OBJECT

public:
    JsonRepositoryFiles(const QString& path, QObject* parent = nullptr);

    QStringList selectIds() const override;
    QList<QJsonObject> selectAll() const override;
    QJsonObject read(const QString& id) const override;

    void save(const QJsonObject& data) override;
    void remove(const QString& id) override;

private:
    const QDir m_dir;
    QFileSystemWatcher m_watcher;
};
} // namespace md::data_source

#endif // JSON_REPOSITORY_FILES_H
