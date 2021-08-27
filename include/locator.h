#ifndef LOCATOR_H
#define LOCATOR_H

#include "i_service.h"

#include <unordered_map>

namespace kjarni::app
{
using ServiceMap = std::unordered_map<size_t, domain::IService*>;

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
} // namespace kjarni::app

#endif // LOCATOR_H
