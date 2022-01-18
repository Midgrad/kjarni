#ifndef VEHICLE_TRAITS_H
#define VEHICLE_TRAITS_H

#include "kjarni_traits.h"
#include "vehicle_type.h"

namespace md::domain
{
namespace props
{
constexpr char online[] = "online";

constexpr char model[] = "model";
} // namespace props

namespace vehicle
{
const VehicleType generic = { "generic", "Generic", "generic_aircraft.svg", "generic_aircraft.glb" };
}

namespace features
{
constexpr char dashboard[] = "dashboard";
}
} // namespace md::domain

#endif // VEHICLE_TRAITS_H
