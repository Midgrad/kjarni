#ifndef GEO_TRAITS_H
#define GEO_TRAITS_H

namespace md::domain
{
// Datums provided in proj4 format
namespace datums
{
constexpr char wgs84[] = "WGS84";
} // namespace datums

constexpr char latitude[] = "latitude";
constexpr char longitude[] = "longitude";
constexpr char altitude[] = "altitude";
constexpr char datum[] = "datum";
} // namespace md::domain

#endif // GEO_TRAITS_H
