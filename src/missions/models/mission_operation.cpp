#include "mission_operation.h"

#include <QDebug>
#include <QMetaEnum>

#include "mission_traits.h"

using namespace md::domain;

MissionOperation::MissionOperation(Type type, Mission* mission, QObject* parent) :
    Entity(mission->id, parent),
    type(type),
    mission(mission),
    state(InProgress, std::bind(&Entity::changed, this)),
    progress(0, std::bind(&Entity::changed, this)),
    total(0, std::bind(&Entity::changed, this))
{
    Q_ASSERT(mission);
}

bool MissionOperation::isComplete() const
{
    return progress >= total;
}

QVariantMap MissionOperation::toVariantMap() const
{
    QVariantMap map = Entity::toVariantMap();
    map[props::progress] = this->progress();
    map[props::total] = this->total();
    map[props::complete] = this->isComplete();
    map[props::type] = QVariant::fromValue(this->type()).toString();
    return map;
}
