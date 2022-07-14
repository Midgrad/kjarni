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

constexpr char connected[] = "connected";
constexpr char online[] = "online";
} // namespace props

namespace comm_link
{
const CommLinkType idle = { "udp", TR_COMM_LINKS("Idle") };

const CommLinkType udp = { "udp", TR_COMM_LINKS("UDP") };
const CommLinkType tcp = { "tcp", TR_COMM_LINKS("TCP") };
const CommLinkType uart = { "uart", TR_COMM_LINKS("UART") };

const QList<const CommLinkType*> defaultTypes = { &udp, &tcp, &uart };

constexpr char port[] = "port";
constexpr char address[] = "address";
constexpr char device[] = "device";
} // namespace comm_link

} // namespace md::domain

#endif // COMM_LINK_TRAITS_Hs
