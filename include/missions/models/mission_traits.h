#ifndef MISSION_TRAITS_H
#define MISSION_TRAITS_H

#include "geo_traits.h"
#include "mission_type.h"

#define TR_MISSION(string) QT_TRANSLATE_NOOP("Route", string)

namespace md::domain
{
namespace props
{
constexpr char vehicle[] = "vehicle";
constexpr char mission[] = "mission";
constexpr char home[] = "home";
constexpr char items[] = "items";
constexpr char item[] = "item";
constexpr char route[] = "route";
constexpr char shortName[] = "shortName";
constexpr char block[] = "block";

constexpr char current[] = "current";
constexpr char reached[] = "reached";
constexpr char complete[] = "complete";
constexpr char progress[] = "progress";
constexpr char total[] = "total";
} // namespace props

namespace mission
{
constexpr float minAltitude = -450.0;   // Dead sea level is -417,5
constexpr float maxAltitude = 100000.0; // Karman line
constexpr float maxDistance = 1000000.0;
constexpr float maxRadius = 1000000.0;
constexpr int maxTime = 100000; // 86400 sec in a day

// Parameters
const ParameterType distance = {
    "distance", TR_MISSION("Distance"), ParameterType::Real, 0.0, 0.0, maxDistance, 1
};
const ParameterType time = { "time", TR_MISSION("Time"), ParameterType::Int, 0, 0, maxTime };
const ParameterType yaw = { "yaw", TR_MISSION("Yaw"), ParameterType::Real, qQNaN(), 0, 360 };
const ParameterType altitude = { "altitude", TR_MISSION("Altitude"), ParameterType::Real,
                                 0,          minAltitude,          maxAltitude };
const ParameterType pitch = { "pitch", TR_MISSION("Pitch"), ParameterType::Real, 15.0, -90.0, 90.0 };
const ParameterType acceptRadius = {
    "accept_radius", TR_MISSION("Accept. radius"), ParameterType::Real, 0, 0, maxRadius
};
const ParameterType radius = {
    "radius", TR_MISSION("Radius"), ParameterType::Real, 100, 0, maxRadius
};
const ParameterType heading = { "heading", TR_MISSION("Heading"), ParameterType::Int, 0, 0, 359 };
const ParameterType spacing = { "spacing", TR_MISSION("Spacing"), ParameterType::Int, 50, 10, 10000 };
const ParameterType doubled = { "doubled", TR_MISSION("Doubled"), false };

} // namespace mission
} // namespace md::domain

#endif // MISSION_TRAITS_H
