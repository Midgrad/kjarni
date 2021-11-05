#ifndef TEST_MISSION_TRAITS_H
#define TEST_MISSION_TRAITS_H

#include "mission_traits.h"

namespace md::domain::test_mission
{
const Parameter radius = { "radius", Parameter::Real, 100 };
const Parameter airspeed = { "airspeed", Parameter::Int, 10 };
const Parameter passthrough = { "passthrough", Parameter::Bool, true };

const WaypointType waypoint = { "waypoint",
                                { &mission::latitude, &mission::longitude, &mission::altitude,
                                  &mission::relative, &airspeed, &passthrough } };
const WaypointType circle = { "circle",
                              { &mission::latitude, &mission::longitude, &mission::altitude,
                                &mission::relative, &airspeed, &radius } };

const RouteType routeType = { "test_route", { &waypoint, &circle } };
const MissionType missionType = { "test_mission", &routeType, &waypoint };

} // namespace md::domain::test_mission

#endif // TEST_MISSION_TRAITS_H
