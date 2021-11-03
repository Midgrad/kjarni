#ifndef ROUTE_TRAITS_H
#define ROUTE_TRAITS_H

#include "kjarni_traits.h"
#include "route_type.h"

namespace md::domain
{
namespace params
{
constexpr char waypoints[] = "waypoints";
constexpr char waypoint[] = "waypoint";
constexpr char route[] = "route";

constexpr char current[] = "current";
constexpr char reached[] = "reached";
constexpr char confirmed[] = "confirmed";
} // namespace params

namespace mission
{
const Parameter latitude = { "latitude", Parameter::Real, 0.0 };
const Parameter longitude = { "longitude", Parameter::Real, 0.0 };
const Parameter altitude = { "altitude", Parameter::Real, 100.0 };
const Parameter relative = { "relative", Parameter::Bool, true };
} // namespace mission
} // namespace md::domain

#endif // ROUTE_TRAITS_H
