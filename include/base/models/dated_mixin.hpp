#ifndef DATED_MIXIN_HPP
#define DATED_MIXIN_HPP

#include "entity.h"

#include <QDateTime>

namespace md::domain
{
template<class Base>
class DatedMixin : public Base
{
public:
    template<typename... Args>
    DatedMixin(const QDateTime& createdAt = QDateTime::currentDateTime(),
               const QDateTime& updatedAt = QDateTime::currentDateTime(), const Args&... args) :
        Base(args...),
        createdAt(createdAt),
        updatedAt(updatedAt, std::bind(&Base::changed, this))
    {
    }

    utils::ConstProperty<QDateTime> createdAt;
    utils::Property<QDateTime> updatedAt;

    QVariantMap toVariantMap() const override
    {
        QVariantMap map = Base::toVariantMap();
        map.insert(props::createdAt, this->createdAt());
        map.insert(props::updatedAt, this->updatedAt());
        return map;
    }

    void fromVariantMap(const QVariantMap& map) override
    {
        updatedAt = map.value(props::updatedAt, this->updatedAt()).toDateTime();
        Base::fromVariantMap(map);
    }
};
} // namespace md::domain

#endif // DATED_MIXIN_HPP
