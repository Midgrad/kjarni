#ifndef KJARNI_TRAITS_H
#define KJARNI_TRAITS_H

#include <QJsonObject>

namespace md::domain
{
namespace props
{
constexpr char id[] = "id";
constexpr char name[] = "name";
constexpr char icon[] = "icon";
constexpr char visible[] = "visible";
constexpr char createdAt[] = "created_at";
constexpr char updatedAt[] = "updated_at";
constexpr char param[] = "param";
constexpr char params[] = "params";
constexpr char value[] = "value";
constexpr char type[] = "type";
constexpr char state[] = "state";
constexpr char position[] = "position";

constexpr char defaultValue[] = "defaultValue";
constexpr char minValue[] = "minValue";
constexpr char maxValue[] = "maxValue";
constexpr char step[] = "step";
constexpr char variants[] = "variants";
} // namespace props
} // namespace md::domain

#endif // KJARNI_TRAITS_H
