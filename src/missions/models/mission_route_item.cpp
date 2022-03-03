#include "mission_route_item.h"

#include <QDebug>

#include "route_traits.h"

using namespace md::domain;

MissionRouteItemType::MissionRouteItemType(const QString& id, const QString& name) :
    id(id),
    name(name)
{
}

MissionRouteItem::MissionRouteItem(MissionRouteItemType* type, const QVariant& id, QObject* parent) :
    NamedMixin<Entity>(type->name, id, parent),
    position(Geodetic(), std::bind(&Entity::changed, this)),
    current(false, std::bind(&Entity::changed, this)),
    reached(false, std::bind(&Entity::changed, this)),
    m_type(type)
{
    this->syncParameters();
}

QVariantMap MissionRouteItem::toVariantMap() const
{
    QVariantMap map = NamedMixin<Entity>::toVariantMap();

    map.insert(props::position, this->position().toVariantMap());
    map.insert(props::current, this->current());
    map.insert(props::reached, this->reached());

    QVariantMap parameterMap;
    for (Parameter* parameter : qAsConst(m_parameters))
    {
        parameterMap.insert(parameter->id().toString(), parameter->value());
    }

    return map;
}

void MissionRouteItem::fromVariantMap(const QVariantMap& map)
{
    position = map.value(props::position, this->position().toVariantMap()).toMap();
    current = map.value(props::current, this->current()).toBool();
    reached = map.value(props::reached, this->reached()).toBool();

    QVariantMap parameterMap = map.value(props::params).toMap();
    for (Parameter* parameter : qAsConst(m_parameters))
    {
        parameter->setAndCheckValue(
            parameterMap.value(parameter->id().toString(), parameter->value));
    }

    NamedMixin<Entity>::fromVariantMap(map);
}

MissionRouteItemType* MissionRouteItem::type() const
{
    return m_type;
}

ParameterList MissionRouteItem::parameters() const
{
    return m_parameters.values();
}

Parameter* MissionRouteItem::parameter(const QString& id) const
{
    return m_parameters.value(id);
}

void MissionRouteItem::setType(MissionRouteItemType* type)
{
    if (m_type == type)
        return;

    m_type = type;
    this->syncParameters();
}

void MissionRouteItem::syncParameters()
{
    ParameterMap newParameters = m_type->createParameters();

    for (const QString& id : newParameters.keys())
    {
        Parameter* newParameter = newParameters[id];
        Parameter* oldParameter = m_parameters.value(id, nullptr);

        // Save old values for same parameters
        if (oldParameter)
            newParameter->setAndCheckValue(oldParameter->value);

        connect(newParameter, &Parameter::changed, this, &MissionRouteItem::changed);
    }

    qDeleteAll(m_parameters);
    m_parameters = newParameters;

    emit changed();
}
