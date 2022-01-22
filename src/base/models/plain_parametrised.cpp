#include "plain_parametrised.h"

using namespace md::domain;

PlainParametrised::PlainParametrised(const QString& name, const QVariant& id,
                                     const QVariantMap& parameters, QObject* parent) :
    NamedMixin<Entity>(name, id, parent),
    m_parameters(parameters)
{
}

PlainParametrised::PlainParametrised(const QVariantMap& map, QObject* parent) :
    PlainParametrised(map.value(props::name).toString(), map.value(props::id, utils::generateId()),
                      map.value(props::params).toMap(), parent)
{
}

const QVariantMap& PlainParametrised::parameters() const
{
    return m_parameters;
}

QVariant PlainParametrised::parameter(const QString& key) const
{
    return m_parameters.value(key);
}

bool PlainParametrised::hasParameter(const QString& key) const
{
    return m_parameters.contains(key);
}

void PlainParametrised::setParameters(const QVariantMap& parameters)
{
    if (m_parameters == parameters)
        return;

    m_parameters = parameters;
    emit changed();
}

void PlainParametrised::setParameter(const QString& key, const QVariant& value)
{
    if (m_parameters.value(key) == value)
        return;

    m_parameters[key] = value;
    emit changed();
}

void PlainParametrised::removeParameters(const QStringList& keys)
{
    bool changedFlag = false;

    for (const QString& key : keys)
    {
        if (m_parameters.remove(key))
        {
            changedFlag = true;
        }
    }

    if (changedFlag)
        emit changed();
}

void PlainParametrised::removeParameter(const QString& key)
{
    if (m_parameters.remove(key))
    {
        emit changed();
    }
}

QVariantMap PlainParametrised::toVariantMap() const
{
    QVariantMap map = NamedMixin<Entity>::toVariantMap();
    map.insert(props::params, m_parameters);
    return map;
}

void PlainParametrised::fromVariantMap(const QVariantMap& map)
{
    m_parameters = map.value(props::params, m_parameters).toMap();
    NamedMixin<Entity>::fromVariantMap(map); // changed will be emited here
}
