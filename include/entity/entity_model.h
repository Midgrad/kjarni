#ifndef ENTITY_MODEL_H
#define ENTITY_MODEL_H

#include <QAbstractListModel>

namespace md
{
namespace presentation
{
template<class Entity>
class EntityModel : public QAbstractListModel
{
    Q_DISABLE_COPY(EntityModel)

public:
    enum EntityRoles
    {
        EntityRole = Qt::UserRole + 1,
        IdRole,
        NameRole,
        ParametersRole,
        JsonRole
    };

    explicit EntityModel(QObject* parent = nullptr);
    ~EntityModel() override;

    int row(Entity* entity);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QList<Entity*> entities() const;

public slots:
    virtual void add(Entity* entity);
    virtual void remove(Entity* entity);
    virtual void reset(const QList<Entity*>& entities);
    virtual void clear();

protected:
    QHash<int, QByteArray> roleNames() const override;

    QList<Entity*> m_entities;
};

} // namespace presentation
} // namespace md

#include "entity_model.hpp"

#endif // ENTITY_MODEL_H
