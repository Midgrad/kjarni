#include "vehicle.h"

using namespace md::domain;

Vehicle::Vehicle(Type type, const QVariant& id, const QString& name, QObject* parent) :
    Entity(id, name, parent),
    m_type(type)
{
}

QString Vehicle::id() const
{
    return Entity::id().toString();
}

Vehicle::Type Vehicle::type() const
{
    return m_type;
}

void Vehicle::setType(Type type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(type);
}
