#ifndef VISIBLE_MIXIN_HPP
#define VISIBLE_MIXIN_HPP

#include "entity.h"

namespace md::domain
{
template<class Base>
class VisibleMixin : public Base
{
public:
    template<typename... Args>
    VisibleMixin(bool visible, const Args&... args) :
        Base(args...),
        visible(visible, std::bind(&Base::changed, this))
    {
    }

    utils::Property<bool> visible;

    QVariantMap toVariantMap() const override
    {
        QVariantMap map = Base::toVariantMap();
        map.insert(props::visible, this->visible());
        return map;
    }

    void fromVariantMap(const QVariantMap& map) override
    {
        visible = map.value(props::visible, this->visible()).toBool();
        Base::fromVariantMap(map);
    }
};
} // namespace md::domain

#endif // VISIBLE_MIXIN_HPP
