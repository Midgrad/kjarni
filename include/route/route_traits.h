#ifndef ROUTE_TRAITS_H
#define ROUTE_TRAITS_H

#include "geo_traits.h"
#include "route_type.h"

namespace md::domain
{
namespace props
{
constexpr char waypoints[] = "waypoints";
constexpr char waypoint[] = "waypoint";
constexpr char route[] = "route";

constexpr char current[] = "current";
constexpr char reached[] = "reached";
constexpr char confirmed[] = "confirmed";
} // namespace params

namespace route
{
const Parameter relativeAlt = { "relative_alt", QT_TRANSLATE_NOOP("Route", "Rel.alt."),
                                Parameter::Bool, true };
const Parameter distance = { "distance", QT_TRANSLATE_NOOP("Route", "Distance"), Parameter::Real,
                             0.0 };
const Parameter time = { "time", QT_TRANSLATE_NOOP("Route", "Time"), Parameter::Int, 0 };
const Parameter yaw = { "yaw", QT_TRANSLATE_NOOP("Route", "Yaw"), Parameter::Real, qQNaN() };
const Parameter pitch = { "pitch", QT_TRANSLATE_NOOP("Route", "Pitch"), Parameter::Real, 15.0 };
const Parameter radius = { "radius", QT_TRANSLATE_NOOP("Route", "Radius"), Parameter::Real, 0 };
} // namespace route
} // namespace md::domain

#endif // ROUTE_TRAITS_H
