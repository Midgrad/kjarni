#ifndef JSON_MODEL_H
#define JSON_MODEL_H

#include <QAbstractItemModel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

class QJsonModel;
class QJsonItem;

class QJsonTreeItem
{
public:
    QJsonTreeItem(QJsonTreeItem* parent = nullptr);
    ~QJsonTreeItem();

    void appendChild(QJsonTreeItem* item);
    QJsonTreeItem* child(int row);
    QJsonTreeItem* parent();
    int childCount() const;
    int row() const;
    void setKey(const QString& key);
    void setValue(const QString& value);
    void setType(const QJsonValue::Type& type);
    QString key() const;
    QString value() const;
    QJsonValue::Type type() const;

    static QJsonTreeItem* load(const QJsonValue& value, QJsonTreeItem* parent = 0);

private:
    QString mKey;
    QString mValue;
    QJsonValue::Type mType;
    QList<QJsonTreeItem*> mChilds;
    QJsonTreeItem* mParent;
};

class QJsonModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QJsonModel(QObject* parent = nullptr);
    QJsonModel(const QByteArray& json, QObject* parent = nullptr);
    ~QJsonModel();
    bool loadJson(const QByteArray& json);
    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex& index, const QVariant& value,
                 int role = Qt::EditRole) Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex& index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex& index) const Q_DECL_OVERRIDE;
    QJsonDocument json() const;

private:
    QJsonValue genJson(QJsonTreeItem*) const;

    QJsonTreeItem* mRootItem;
    QStringList mHeaders;
};

#endif // QJSONMODEL_H
