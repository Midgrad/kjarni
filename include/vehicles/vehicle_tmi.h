#ifndef VEHICLE_TMI_H
#define VEHICLE_TMI_H

namespace md::tmi
{
// Global
constexpr char name[] = "name";

// System
constexpr char state[] = "state";
constexpr char online[] = "online";

// Arming
constexpr char armed[] = "armed";
constexpr char setArmed[] = "setArmed";
constexpr char setArmedStatus[] = "setArmedStatus";

// Modes
constexpr char mode[] = "mode";
constexpr char modes[] = "modes";
constexpr char setMode[] = "setMode";
constexpr char setModeStatus[] = "setModeStatus";

// Devices
constexpr char devices[] = "devices";
constexpr char enabled[] = "enabled";
constexpr char present[] = "present";
constexpr char health[] = "health";

// Attitude
constexpr char pitch[] = "pitch";
constexpr char roll[] = "roll";
constexpr char yaw[] = "yaw";
constexpr char heading[] = "heading";
constexpr char course[] = "course";
constexpr char desiredRoll[] = "desiredRoll";
constexpr char desiredPitch[] = "desiredPitch";
constexpr char desiredHeading[] = "desiredHeading";

// Speeds
constexpr char gs[] = "gs";
constexpr char ias[] = "ias";
constexpr char tas[] = "tas";

// Altitudes
constexpr char climb[] = "climb";
constexpr char altitudeAmsl[] = "altitudeAmsl";
constexpr char altitudeRelative[] = "altitudeRelative";
constexpr char altitudeTerrain[] = "altitudeTerrain";

// Position
constexpr char latitude[] = "latitude";
constexpr char longitude[] = "longitude";
constexpr char x[] = "x";
constexpr char y[] = "y";

// Home
constexpr char homeLatitude[] = "homeLatitude";
constexpr char homeLongitude[] = "homeLongitude";
constexpr char homeAltitude[] = "homeAltitude";

// Target
constexpr char wpDistance[] = "wpDistance";
constexpr char targetBearing[] = "targetBearing";

} // namespace md::tmi

#endif // VEHICLE_TMI_H
