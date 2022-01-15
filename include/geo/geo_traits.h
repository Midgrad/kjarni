#ifndef GEO_TRAITS_H
#define GEO_TRAITS_H

#include "kjarni_traits.h"

namespace md::domain::geo
{
// Datums provided in proj4 format
namespace datums
{
constexpr char wgs84[] = "WGS84";
} // namespace datums

constexpr char x[] = "x";
constexpr char y[] = "y";
constexpr char z[] = "z";

constexpr char latitude[] = "latitude";
constexpr char longitude[] = "longitude";
constexpr char altitude[] = "altitude";
constexpr char datum[] = "datum";
} // namespace md::domain::geo

#endif // GEO_TRAITS_H
