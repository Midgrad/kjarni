#include "mission.h"

using namespace md::domain;

Mission::Mission(const MissionType& type, const QVariant& id, const QString& name, QObject* parent) :
    Entity(id, name, parent),
    m_type(type),
    m_route(new Route(&type.routeType, this))
{
}

Mission::Mission(const QJsonObject& json, const MissionType& type, QObject* parent) :
    Entity(json, parent),
    m_type(type),
    m_vehicle(json.value(params::vehicle).toString()),
    m_route(new Route(json.value(params::route).toObject(), &type.routeType, this))
{
}

QJsonObject Mission::toJson(bool recursive) const
{
    QJsonObject json = Entity::toJson();

    json.insert(params::type, m_type.name);
    json.insert(params::vehicle, m_vehicle);
    if (recursive)
        json.insert(params::route, m_route->toJson(recursive));

    return json;
}

void Mission::fromJson(const QJsonObject& json)
{
    if (json.contains(params::vehicle))
        this->setVehicle(json.value(params::vehicle).toString());

    if (json.contains(params::route))
        m_route->fromJson(json.value(params::route).toObject());

    Entity::fromJson(json);
}

const MissionType& Mission::type() const
{
    return m_type;
}

QString Mission::vehicle() const
{
    return m_vehicle;
}

Route* Mission::route() const
{
    return m_route;
}

void Mission::setVehicle(const QString& vehicle)
{
    if (m_vehicle == vehicle)
        return;

    m_vehicle = vehicle;
    emit vehicleChanged(vehicle);
}
