#include "entity_model.h"

namespace md
{
namespace presentation
{
template<class Entity>
EntityModel<Entity>::EntityModel(QObject* parent) : QAbstractListModel(parent)
{
}

template<class Entity>
EntityModel<Entity>::~EntityModel()
{
}

template<class Entity>
int EntityModel<Entity>::row(Entity* entity)
{
    return m_entities.indexOf(entity);
}

template<class Entity>
int EntityModel<Entity>::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_entities.count();
}

template<class Entity>
QVariant EntityModel<Entity>::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_entities.count())
        return QVariant();

    Entity* entity = m_entities.at(index.row());

    switch (role)
    {
    case EntityRole:
        return QVariant::fromValue(entity);
    case IdRole:
        return entity->id();
    case NameRole:
        return entity->name();
    case ParametersRole:
        return entity->parameters();
    case JsonRole:
        return entity->toJson();
    }
    return QVariant();
}

template<class Entity>
void EntityModel<Entity>::add(Entity* entity)
{
    this->beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount());

    m_entities.append(entity);

    this->endInsertRows();
}

template<class Entity>
void EntityModel<Entity>::remove(Entity* entity)
{
    int row = m_entities.indexOf(entity);
    if (row == -1)
        return;

    this->beginRemoveRows(QModelIndex(), row, row);

    m_entities.removeAt(row);

    this->endRemoveRows();
}

template<class Entity>
void EntityModel<Entity>::reset(const QList<Entity*>& entities)
{
    this->beginResetModel();

    m_entities = entities;

    this->endResetModel();
}

template<class Entity>
void EntityModel<Entity>::clear()
{
    this->beginResetModel();

    m_entities.clear();

    this->endResetModel();
}

template<class Entity>
QHash<int, QByteArray> EntityModel<Entity>::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[EntityRole] = "entity";
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[ParametersRole] = "params";
    roles[JsonRole] = "json";

    return roles;
}

template<class Entity>
QList<Entity*> EntityModel<Entity>::entities() const
{
    return m_entities;
}

} // namespace presentation
} // namespace md
