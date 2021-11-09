#ifndef TEST_MISSION_TRAITS_H
#define TEST_MISSION_TRAITS_H

#include "mission_traits.h"

namespace md::domain::test_mission
{
const Parameter radius = { "radius", "Radius", Parameter::Real, 100 };
const Parameter airspeed = { "airspeed", "Airspeed", Parameter::Int, 10 };
const Parameter passthrough = { "passthrough", "Passthrough", Parameter::Bool, true };

const RouteItemType waypoint = { "waypoint",
                                "Waypoint",
                                "WPT",
                                { &route::latitude, &route::longitude, &route::altitude,
                                  &route::relativeAlt, &airspeed, &passthrough } };
const RouteItemType circle = { "circle",
                              "Circle",
                              "CRL",
                              { &route::latitude, &route::longitude, &route::altitude,
                                &route::relativeAlt, &airspeed, &radius } };

const RouteType routeType = { "test_route", "Test Route", { &waypoint, &circle } };
const MissionType missionType = { "test_mission", "Test Mission", &routeType, &waypoint };

} // namespace md::domain::test_mission

#endif // TEST_MISSION_TRAITS_H
