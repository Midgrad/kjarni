#ifndef I_JSON_GATEWAY_H
#define I_JSON_GATEWAY_H

#include <QJsonObject>
#include <QObject>

namespace md::data_source
{
class IJsonGateway : public QObject
{
    Q_OBJECT

public:
    IJsonGateway(QObject* parent = nullptr) : QObject(parent)
    {
    }

    virtual QVariantList selectIds() const = 0;
    virtual QList<QJsonObject> readAll() const = 0;
    virtual QJsonObject read(const QVariant& id) const = 0;

    virtual void save(const QVariant& id, const QJsonObject& data) = 0;
    virtual void remove(const QVariant& id) = 0;

signals:
    void itemsChanged();
};
} // namespace md::data_source

#endif // I_JSON_GATEWAY_H
