#ifndef COMM_LINK_TRAITS_H
#define COMM_LINK_TRAITS_H

#include "comm_link_type.h"
#include "kjarni_traits.h"

#define TR_COMM_LINKS(string) QT_TRANSLATE_NOOP("CommLinks", string)

namespace md::domain
{
namespace props
{
constexpr char protocol[] = "protocol";
constexpr char autoconnect[] = "autoconnect";

constexpr char connected[] = "connected";
constexpr char online[] = "online";
} // namespace props

} // namespace md::domain

#endif // COMM_LINK_TRAITS_Hs
