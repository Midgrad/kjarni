#ifndef MISSION_TRAITS_H
#define MISSION_TRAITS_H

#include "mission_type.h"
#include "route_traits.h"

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

constexpr char current[] = "current";
constexpr char reached[] = "reached";
constexpr char confirmed[] = "confirmed";
} // namespace props
} // namespace md::domain

#endif // MISSION_TRAITS_H
