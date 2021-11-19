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

// WaypointItems
const WaypointItemType changeSpeed{ "ch_speed", "Change speed", "CH SPD", { &airspeed } };
const WaypointItemType changeAltitude{ "ch_speed", "Change alt", "CH ALT", { &altitude } };
const WaypointItemType takePhoto{ "take_photo", "Take photo", "PHOTO", {} };

// Waypoints
const WaypointType waypoint = { "waypoint",
                                "Waypoint",
                                "WPT",
                                { &route::relativeAlt, &passthrough },
                                { &changeSpeed, &changeAltitude, &takePhoto } };
const WaypointType circle = { "circle", "Circle", "CRL", { &route::relativeAlt, &radius }, {} };
const WaypointType loop = {
    "circle", "Circle", "CRL", { &route::relativeAlt, &radius }, { &changeAltitude }
};

// Routes
const RouteType routeType = { "test_route", "Test Route", { &waypoint, &circle, &loop } };

// Mission
const MissionType missionType = { "test_mission", "Test Mission", &routeType, &waypoint };

} // namespace md::domain::test_mission

#endif // TEST_MISSION_TRAITS_H
