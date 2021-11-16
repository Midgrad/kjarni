#ifndef ROUTE_TRAITS_H
#define ROUTE_TRAITS_H

#include "geo_traits.h"
#include "route_type.h"

#define TR_ROUTE(string) QT_TRANSLATE_NOOP("Route", string)

namespace md::domain
{
namespace props
{
constexpr char defaultValue[] = "defaultValue";
constexpr char minValue[] = "minValue";
constexpr char maxValue[] = "maxValue";
constexpr char step[] = "step";

constexpr char waypoints[] = "waypoints";
constexpr char waypoint[] = "waypoint";
constexpr char route[] = "route";

constexpr char current[] = "current";
constexpr char reached[] = "reached";
constexpr char confirmed[] = "confirmed";
} // namespace props

namespace route
{
constexpr float minAltitude = -450.0;   // Dead sea level is -417,5
constexpr float maxAltitude = 100000.0; // Karman line
constexpr float maxDistance = 1000000.0;
constexpr float maxRadius = 1000000.0;
constexpr int maxTime = 100000; // 86400 sec in a day

const Parameter relativeAlt = { "relative_alt", TR_ROUTE("Rel.alt."), false };
const Parameter distance = {
    "distance", TR_ROUTE("Distance"), Parameter::Real, 0.0, 0.0, maxDistance, 1
};
const Parameter time = { "time", TR_ROUTE("Time"), Parameter::Int, 0, 0, maxTime };
const Parameter yaw = { "yaw", TR_ROUTE("Yaw"), Parameter::Real, qQNaN(), 0, 360 };
const Parameter pitch = { "pitch", TR_ROUTE("Pitch"), Parameter::Real, 15.0, -90.0, 90.0 };
const Parameter acceptRadius = { "accept_radius", TR_ROUTE("Accept. radius"), Parameter::Real, 0, 0,
                                 maxRadius };
const Parameter radius = { "radius", TR_ROUTE("Radius"), Parameter::Real, 100, 0, maxRadius };
} // namespace route
} // namespace md::domain

#endif // ROUTE_TRAITS_H
