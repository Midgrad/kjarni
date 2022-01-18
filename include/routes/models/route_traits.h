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
constexpr char calcData[] = "calcData";
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
const Parameter distance = {
    "distance", TR_ROUTE("Distance"), Parameter::Real, 0.0, 0.0, maxDistance, 1
};
const Parameter time = { "time", TR_ROUTE("Time"), Parameter::Int, 0, 0, maxTime };
const Parameter yaw = { "yaw", TR_ROUTE("Yaw"), Parameter::Real, qQNaN(), 0, 360 };
const Parameter altitude = { "altitude", TR_ROUTE("Altitude"), Parameter::Real,
                             0,          minAltitude,          maxAltitude };
const Parameter pitch = { "pitch", TR_ROUTE("Pitch"), Parameter::Real, 15.0, -90.0, 90.0 };
const Parameter acceptRadius = { "accept_radius", TR_ROUTE("Accept. radius"), Parameter::Real, 0, 0,
                                 maxRadius };
const Parameter radius = { "radius", TR_ROUTE("Radius"), Parameter::Real, 100, 0, maxRadius };
const Parameter heading = { "heading", TR_ROUTE("Heading"), Parameter::Int, 0, 0, 359 };
const Parameter spacing = { "spacing", TR_ROUTE("Spacing"), Parameter::Int, 50, 10, 10000 };
const Parameter doubled = { "doubled", TR_ROUTE("Doubled"), false };
} // namespace route
} // namespace md::domain

#endif // ROUTE_TRAITS_H
