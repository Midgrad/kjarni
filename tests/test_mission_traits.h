#ifndef TEST_MISSION_TRAITS_H
#define TEST_MISSION_TRAITS_H

#include "mission_traits.h"

namespace md::domain::test_mission
{
// Prameters
const Parameter radius = { "radius", "Radius", Parameter::Real, 100, 0, route::maxRadius };
const Parameter altitude = { "altitude", "Altitude",         Parameter::Real,
                             0.0,        route::minAltitude, route::maxAltitude };
const Parameter airspeed = { "airspeed", "Airspeed", Parameter::Int, 10, 0, 100 };
const Parameter passthrough = { "passthrough", "Passthrough", true };

// Route Items
const RouteItemType changeSpeed{ "ch_speed", "Change speed", "CH SPD", { &airspeed } };
const RouteItemType changeAltitude{ "ch_speed", "Change alt", "CH ALT", { &altitude } };
const RouteItemType takePhoto{ "take_photo", "Take photo", "PHOTO", {} };

const RouteItemType waypoint = { "waypoint", "Waypoint", "WPT", { &passthrough } };
const RouteItemType circle = { "circle", "Circle", "CRL", { &radius } };
const RouteItemType loop = { "circle", "Circle", "CRL", { &radius } };

// Routes
const RouteType routeType = { "test_route",
                              "Test Route",
                              { &waypoint, &circle, &loop, &changeSpeed, &changeAltitude,
                                &takePhoto },
                              {} };

// Mission
const MissionType missionType = { "test_mission", "Test Mission", &routeType, &waypoint, &waypoint };

} // namespace md::domain::test_mission

#endif // TEST_MISSION_TRAITS_H
