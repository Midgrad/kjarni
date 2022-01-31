#ifndef PARAMETRISED_MIXIN_HPP
#define PARAMETRISED_MIXIN_HPP

#include "entity.h"

namespace md::domain
{
template<class Base>
class ParametrisedMixin : public Base
{
public:
    template<typename... Args>
    ParametrisedMixin(
        const QVariantMap& parameters, const std::function<void()>& notifier = []() {},
        const Args&... args) :
        Base(args...),
        m_parameters(parameters),
        m_notifier(notifier)
    {
    }

    QVariant parameter(const QString& key) const
    {
        return m_parameters.value(key);
    }

    const QVariantMap& parameters() const
    {
        return m_parameters;
    }

    bool hasParameter(const QString& key) const
    {
        return m_parameters.contains(key);
    }

    void setParameter(const QString& key, const QVariant& value)
    {
        if (m_parameters.value(key) == value)
            return;

        m_parameters[key] = value;
        m_notifier();
    }

    void setParameters(const QVariantMap& parameters)
    {
        if (m_parameters == parameters)
            return;

        m_parameters = parameters;
        m_notifier();
    }

    void removeParameter(const QString& key)
    {
        if (m_parameters.remove(key))
        {
            m_notifier();
        }
    }

    void removeParameters(const QStringList& keys)
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
            m_notifier();
    }

    QVariantMap toVariantMap() const override
    {
        QVariantMap map = Base::toVariantMap();
        map.insert(props::params, m_parameters);
        return map;
    }

    void fromVariantMap(const QVariantMap& map) override
    {
        m_parameters = map.value(props::params, m_parameters).toMap();
        Base::fromVariantMap(map);
        m_notifier();
    }

private:
    QVariantMap m_parameters;
    std::function<void()> m_notifier;
};
} // namespace md::domain

#endif // PARAMETRISED_MIXIN_HPP
