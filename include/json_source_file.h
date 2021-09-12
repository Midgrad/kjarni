#ifndef JSON_SOURCE_FILE_H
#define JSON_SOURCE_FILE_H

#include "i_json_source.h"

namespace md::data_source
{
class JsonSourceFile : public IJsonSource
{
public:
    JsonSourceFile(const QString& fileName);

    QString errorString() const override;
    QJsonDocument read() override;

    void save(const QJsonDocument& document) override;

private:
    const QString m_fileName;
    QString m_errorString;
};
} // namespace md::data_source

#endif // JSON_SOURCE_FILE_H
