#ifndef LOCATOR_H
#define LOCATOR_H

#include "traits.h"

#include <unordered_map>

namespace kjarni::domain
{
using ServiceMap = std::unordered_map<size_t, IService*>;

class Locator
{
public:
    template<typename ServiceType>
    static void provide(ServiceType* service)
    {
        m_services[typeid(ServiceType).hash_code()] = service;
    }

    template<typename ServiceType>
    static ServiceType* get()
    {
        return static_cast<ServiceType*>(m_services[typeid(ServiceType).hash_code()]);
    }

private:
    Locator() = default;

    static ServiceMap m_services;
};
} // namespace kjarni::domain

#endif // LOCATOR_H