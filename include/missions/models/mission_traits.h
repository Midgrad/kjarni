#ifndef MISSION_TRAITS_H
#define MISSION_TRAITS_H

#include "route_traits.h"

#define TR_MISSION(string) QT_TRANSLATE_NOOP("Mission", string)

namespace md::domain
{
namespace props
{
constexpr char vehicle[] = "vehicle";
constexpr char mission[] = "mission";
constexpr char home[] = "home";

constexpr char complete[] = "complete";
constexpr char progress[] = "progress";
constexpr char total[] = "total";
} // namespace props

namespace md::domain::mission
{
// Parameters
constexpr char abortAltitude[] = "abortAltitude";
} // namespace md::domain::mission
} // namespace md::domain

#endif // MISSION_TRAITS_H
