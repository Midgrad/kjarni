#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "entity.h"
#include "kjarni_traits.h"

using namespace md::domain;

struct EntityTestArgs
{
    QVariant id;
    QString name;
    QVariantMap params;
};

class EntityTest : public ::testing::TestWithParam<EntityTestArgs>
{
public:
    EntityTest() = default;
};

INSTANTIATE_TEST_SUITE_P(
    instantiation, EntityTest,
    ::testing::Values(
        EntityTestArgs({ md::utils::generateId(), "Name 665", {} }),
        EntityTestArgs({ md::utils::generateId(), "some_name", { { "bool_propery", false } } }),
        EntityTestArgs({ md::utils::generateId(),
                         "entity",
                         { { "int_propery", 34 }, { "float_propery", 34 } } }),
        EntityTestArgs({ md::utils::generateId(), "", { { "string_propery", "str123" } } })));

TEST_P(EntityTest, testParameters)
{
    EntityTestArgs args = GetParam();
    Entity entity(GetParam().id, GetParam().name, GetParam().params);

    QSignalSpy spy(&entity, &Entity::changed);

    EXPECT_EQ(entity.parameters(), args.params);
    EXPECT_EQ(spy.count(), 0);

    entity.setParameters(
        { { "bool_propery", true }, { "int_propery", -42 }, { "string_propery", "test" } });

    EXPECT_EQ(entity.parameter("bool_propery"), true);
    EXPECT_EQ(entity.parameter("int_propery"), -42);
    EXPECT_EQ(entity.parameter("string_propery"), "test");

    EXPECT_EQ(spy.count(), 1);

    entity.removeParameter("string_propery");

    EXPECT_EQ(entity.parameter("string_propery"), QVariant());
    EXPECT_EQ(spy.count(), 2);

    entity.removeParameters({ "bool_propery", "int_propery" });

    EXPECT_TRUE(entity.parameters().isEmpty());
    EXPECT_EQ(spy.count(), 3);
}

TEST_P(EntityTest, testConstructFromMap)
{
    EntityTestArgs args = GetParam();

    QVariantMap map;
    map.insert(params::id, args.id.toString());
    map.insert(params::name, args.name);
    map.insert(params::params, QJsonValue::fromVariant(args.params));

    Entity entity(map);

    EXPECT_EQ(entity.id(), args.id);
    EXPECT_EQ(entity.name(), args.name);
    EXPECT_EQ(entity.parameters(), args.params);
}

TEST_P(EntityTest, testFromVariant)
{
    EntityTestArgs args = GetParam();
    Entity entity(args.id, QString());

    QVariantMap map;
    map.insert(params::name, args.name);
    map.insert(params::params, QJsonValue::fromVariant(args.params));

    entity.fromVariantMap(map);
    EXPECT_EQ(entity.id(), args.id);
    EXPECT_EQ(entity.name(), args.name);
    EXPECT_EQ(entity.parameters(), args.params);
}

TEST_P(EntityTest, testToVariant)
{
    EntityTestArgs args = GetParam();
    Entity entity(GetParam().id, GetParam().name, GetParam().params);

    QVariantMap map;
    map.insert(params::id, args.id);
    if (!args.name.isNull())
        map.insert(params::name, args.name);
    if (!args.params.isEmpty())
        map.insert(params::params, args.params);

    EXPECT_EQ(map, entity.toVariantMap());
}
