#ifndef I_JSON_REPOSITORY_H
#define I_JSON_REPOSITORY_H

#include <QJsonObject>
#include <QObject>

namespace md::data_source
{
class IJsonRepository : public QObject
{
    Q_OBJECT

public:
    IJsonRepository(QObject* parent = nullptr) : QObject(parent)
    {
    }

    virtual QStringList selectIds() const = 0;
    virtual QList<QJsonObject> selectAll() const = 0;
    virtual QJsonObject read(const QString& id) const = 0;

    virtual void save(const QJsonObject& data) = 0;
    virtual void remove(const QString& id) = 0;

signals:
    void itemsChanged();
};
} // namespace md::data_source
#endif // I_JSON_REPOSITORY_H
