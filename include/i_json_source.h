#ifndef I_JSON_SOURCE_H
#define I_JSON_SOURCE_H

#include <QJsonDocument>

namespace md::data_source
{
class IJsonSource
{
public:
    IJsonSource() = default;
    virtual ~IJsonSource() = default;

    virtual QString errorString() const = 0;
    virtual QJsonDocument read() = 0;

    virtual void save(const QJsonDocument& document) = 0;
};
} // namespace md::data_source

#endif // I_JSON_SOURCE_H
