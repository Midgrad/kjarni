#ifndef JSON_GATEWAY_FILES_H
#define JSON_GATEWAY_FILES_H

#include "i_json_gateway.h"

#include <QDir>
#include <QFileSystemWatcher>
#include <QMap>

namespace md::data_source
{
class JsonGatewayFiles : public IJsonGateway
{
    Q_OBJECT

public:
    JsonGatewayFiles(const QString& path, QObject* parent = nullptr);

    QVariantList selectIds() const override;
    QList<QJsonObject> readAll() const override;
    QJsonObject read(const QVariant& id) const override;

    void save(const QVariant& id, const QJsonObject& data) override;
    void remove(const QVariant& id) override;

private:
    const QDir m_dir;
    QFileSystemWatcher m_watcher;
};
} // namespace md::data_source

#endif // JSON_GATEWAY_FILES_H
