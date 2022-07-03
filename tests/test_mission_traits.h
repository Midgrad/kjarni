#ifndef TEST_MISSION_TRAITS_H
#define TEST_MISSION_TRAITS_H

#include "mission_traits.h"

namespace md::domain::test_mission
{
// Parameters
const ParameterType radius = { "radius", "Radius", ParameterType::Real, 100, 0, mission::maxRadius };
const ParameterType altitude = { "altitude", "Altitude",         ParameterType::Real,
                                 0.0,        mission::minAltitude, mission::maxAltitude };
const ParameterType airspeed = { "airspeed", "Airspeed", ParameterType::Int, 10, 0, 100 };
const ParameterType passthrough = { "passthrough", "Passthrough", true };

// Mission Items
const MissionItemType changeSpeed{
    "ch_speed", "Change speed", "CH SPD", Positioned::Optional, { &airspeed }
};
const MissionItemType changeAltitude{
    "ch_speed", "Change alt", "CH ALT", Positioned::Optional, { &altitude }
};
const MissionItemType takePhoto{ "take_photo", "Take photo", "PHOTO", Positioned::No, {} };

const MissionItemType waypoint = {
    "waypoint", "Waypoint", "WPT", Positioned::Required, { &passthrough }
};
const MissionItemType circle = { "circle", "Circle", "CRL", Positioned::Required, { &radius } };
const MissionItemType loop = { "circle", "Circle", "CRL", Positioned::Required, { &radius } };

// Missions
const MissionType missionType = { "test_mission",
                                  "Test Mission",
                                  &waypoint,
                                  { &waypoint, &circle, &loop, &changeSpeed, &changeAltitude,
                                    &takePhoto },
                                  {} };
} // namespace md::domain::test_mission

#endif // TEST_MISSION_TRAITS_H
