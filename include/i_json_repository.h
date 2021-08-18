#ifndef I_JSON_REPOSITORY_H
#define I_JSON_REPOSITORY_H

#include <QJsonObject>
#include <QObject>

namespace kjarni::domain
{
namespace json_params
{
constexpr char id[] = "id";
constexpr char name[] = "name";
} // namespace json_params

class IJsonRepository : public QObject
{
    Q_OBJECT

public:
    IJsonRepository(QObject* parent = nullptr) : QObject(parent)
    {
    }

    virtual QStringList ids() const = 0;
    virtual QList<QJsonObject> values() const = 0;
    virtual QJsonObject value(const QString& id) const = 0;

    virtual void save(const QJsonObject& data) = 0;
    virtual void remove(const QString& id) = 0;

signals:
    void itemsChanged();
};
} // namespace kjarni::domain
#endif // I_JSON_REPOSITORY_H
