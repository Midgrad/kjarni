#ifndef ROUTE_TRAITS_H
#define ROUTE_TRAITS_H

#include "geo_traits.h"
#include "route_type.h"

#define TR_ROUTE(string) QT_TRANSLATE_NOOP("Route", string)

namespace md::domain
{
namespace props
{
constexpr char items[] = "items";
constexpr char item[] = "item";
constexpr char route[] = "route";
constexpr char shortName[] = "shortName";
constexpr char block[] = "block";

constexpr char current[] = "current";
constexpr char reached[] = "reached";
} // namespace props

namespace route
{
constexpr float minAltitude = -450.0;   // Dead sea level is -417,5
constexpr float maxAltitude = 100000.0; // Karman line
constexpr float maxDistance = 1000000.0;
constexpr float maxRadius = 1000000.0;
constexpr int maxTime = 100000; // 86400 sec in a day

// Parameters
const ParameterType distance = {
    "distance", TR_ROUTE("Distance"), ParameterType::Real, 0.0, 0.0, maxDistance, 1
};
const ParameterType time = { "time", TR_ROUTE("Time"), ParameterType::Int, 0, 0, maxTime };
const ParameterType yaw = { "yaw", TR_ROUTE("Yaw"), ParameterType::Real, qQNaN(), 0, 360 };
const ParameterType altitude = { "altitude", TR_ROUTE("Altitude"), ParameterType::Real,
                                 0,          minAltitude,          maxAltitude };
const ParameterType pitch = { "pitch", TR_ROUTE("Pitch"), ParameterType::Real, 15.0, -90.0, 90.0 };
const ParameterType acceptRadius = {
    "accept_radius", TR_ROUTE("Accept. radius"), ParameterType::Real, 0, 0, maxRadius
};
const ParameterType radius = {
    "radius", TR_ROUTE("Radius"), ParameterType::Real, 100, 0, maxRadius
};
const ParameterType heading = { "heading", TR_ROUTE("Heading"), ParameterType::Int, 0, 0, 359 };
const ParameterType spacing = { "spacing", TR_ROUTE("Spacing"), ParameterType::Int, 50, 10, 10000 };
const ParameterType doubled = { "doubled", TR_ROUTE("Doubled"), false };
} // namespace route
} // namespace md::domain

#endif // ROUTE_TRAITS_H
