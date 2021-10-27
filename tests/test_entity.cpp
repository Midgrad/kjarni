#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "entity.h"
#include "kjarni_traits.h"

using namespace md::domain;

struct EntityTestArgs
{
    QUuid id;
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
        EntityTestArgs({ QUuid::createUuid(), "Name 665", {} }),
        EntityTestArgs({ QUuid::createUuid(), "some_name", { { "bool_propery", false } } }),
        EntityTestArgs(
            { QUuid::createUuid(), "entity", { { "int_propery", 34 }, { "float_propery", 34 } } }),
        EntityTestArgs({ QUuid::createUuid(), "", { { "string_propery", "str123" } } })));

TEST_P(EntityTest, testParameters)
{
    EntityTestArgs args = GetParam();
    Entity entity(GetParam().id, GetParam().name, GetParam().params);

    QSignalSpy oneSpy(&entity, &Entity::parameterChanged);
    QSignalSpy manySpy(&entity, &Entity::parametersChanged);

    EXPECT_EQ(entity.parameters(), args.params);
    EXPECT_EQ(oneSpy.count(), 0);
    EXPECT_EQ(manySpy.count(), 0);

    entity.setParameters(
        { { "bool_propery", true }, { "int_propery", -42 }, { "string_propery", "test" } });

    EXPECT_EQ(entity.parameter("bool_propery"), true);
    EXPECT_EQ(entity.parameter("int_propery"), -42);
    EXPECT_EQ(entity.parameter("string_propery"), "test");

    EXPECT_EQ(oneSpy.count(), 3);
    EXPECT_EQ(manySpy.count(), 1);

    entity.removeParameter("string_propery");

    EXPECT_EQ(entity.parameter("string_propery"), QVariant());
    EXPECT_EQ(oneSpy.count(), 4);
    EXPECT_EQ(manySpy.count(), 2);

    entity.removeParameters({ "bool_propery", "int_propery" });

    EXPECT_TRUE(entity.parameters().isEmpty());
    EXPECT_EQ(oneSpy.count(), 6);
    EXPECT_EQ(manySpy.count(), 3);
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
