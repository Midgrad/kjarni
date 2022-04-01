#ifndef LOCATOR_H
#define LOCATOR_H

#include "i_service.h"

#include <typeinfo>
#include <unordered_map>

namespace md::app
{
using ServiceMap = std::unordered_map<std::size_t, domain::IService*>;

class Locator
{
public:
    template<typename ServiceType>
    static void provide(ServiceType* service)
    {
        m_services[typeid(ServiceType).hash_code()] = service;
    }

    template<typename ServiceType>
    static void unprovide()
    {
        m_services.erase(typeid(ServiceType).hash_code());
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
} // namespace md::app

#endif // LOCATOR_H
