#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>
#include <QUuid>

#include "entity.h"
#include "kjarni_traits.h"

using namespace md::domain;

struct TestArgs
{
    QVariant id;
    QString name;
    QVariantMap params;
};

class EntityTest : public ::testing::TestWithParam<TestArgs>
{
public:
    EntityTest() = default;
};

INSTANTIATE_TEST_SUITE_P(
    instantiation, EntityTest,
    ::testing::Values(TestArgs({ "text_id", "Name 665", {} }),
                      TestArgs({ 56, "some_name", QVariantMap({ { "bool_propery", false } }) }),
                      TestArgs({ 0, "entity", { { "int_propery", 34 }, { "float_propery", 34 } } }),
                      TestArgs({ "123", "", { { "string_propery", "str123" } } })));

TEST_P(EntityTest, testParameters)
{
    TestArgs args = GetParam();

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
}

TEST_P(EntityTest, testFromJson)
{
    TestArgs args = GetParam();

    QJsonObject json;
    json.insert(md::params::id, QJsonValue::fromVariant(args.id));
    json.insert(md::params::name, args.name);
    json.insert(md::params::params, QJsonValue::fromVariant(args.params));

    Entity entity(json);

    EXPECT_EQ(entity.id(), args.id);
    EXPECT_EQ(entity.name(), args.name);
    EXPECT_EQ(entity.parameters(), args.params);
}

TEST_P(EntityTest, testToJson)
{
    TestArgs args = GetParam();

    Entity entity(GetParam().id, GetParam().name, GetParam().params);

    QJsonObject json;
    json.insert(md::params::id, QJsonValue::fromVariant(args.id));
    json.insert(md::params::name, args.name);
    json.insert(md::params::params, QJsonValue::fromVariant(args.params));

    EXPECT_EQ(json, entity.toJson());
}
