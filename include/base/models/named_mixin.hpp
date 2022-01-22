#ifndef NAMED_MIXIN_HPP
#define NAMED_MIXIN_HPP

#include "entity.h"

namespace md::domain
{
template<class Base>
class NamedMixin : public Base
{
public:
    template<typename... Args>
    NamedMixin(const QString& name, const Args&... args) :
        Base(args...),
        name(name, std::bind(&Base::changed, this))
    {
    }

    utils::Property<QString> name;

    QVariantMap toVariantMap() const override
    {
        QVariantMap map = Base::toVariantMap();
        map.insert(props::name, this->name());
        return map;
    }

    void fromVariantMap(const QVariantMap& map) override
    {
        name = map.value(props::name, this->name()).toString();
        Base::fromVariantMap(map);
    }
};
} // namespace md::domain

#endif // NAMED_MIXIN_HPP
