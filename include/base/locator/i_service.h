#ifndef I_SERVICE_H
#define I_SERVICE_H

#include "export.h"

namespace md::domain
{
class KJARNI_EXPORT IService
{
public:
    IService() = default;
    virtual ~IService() = default;
};
} // namespace md::domain
#endif // I_SERVICE_H
