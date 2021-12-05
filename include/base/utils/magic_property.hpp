#ifndef MAGIC_PROPERTY_HPP
#define MAGIC_PROPERTY_HPP

#include <functional>
#include <type_traits>

namespace md::utils
{
template<typename T>
class ConstProperty
{
public:
    ConstProperty(const T& value = T()) : m_value(value)
    {
    }

    const T& get() const
    {
        return m_value;
    }

    operator const T&() const
    {
        return this->get();
    }

protected:
    T m_value;
};

template<typename T>
class Property : public ConstProperty<T>
{
public:
    Property(
        const T& value = T(), const std::function<void()>& notifier = []() {}) :
        ConstProperty<T>(value),
        m_notifier(notifier)
    {
    }

    void set(const T& value)
    {
        if (ConstProperty<T>::m_value == value)
            return;

        ConstProperty<T>::m_value = value;
        m_notifier();
    }

    Property<T>& operator=(const T& value)
    {
        this->set(value);
        return *this;
    }

private:
    std::function<void()> m_notifier;
};
} // namespace md::utils

#endif // MAGIC_PROPERTY_HPP
