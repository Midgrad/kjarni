#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "kjarni_traits.h"
#include "plain_parametrised.h"

using namespace md::domain;

struct PlainParametrisedTestArgs
{
    QVariant id;
    QString name;
    QVariantMap params;
};

class PlainParametrisedTest : public ::testing::TestWithParam<PlainParametrisedTestArgs>
{
public:
    PlainParametrisedTest() = default;
};

INSTANTIATE_TEST_SUITE_P(
    instantiation, PlainParametrisedTest,
    ::testing::Values(
        PlainParametrisedTestArgs({ md::utils::generateId(), "Name 665", {} }),
        PlainParametrisedTestArgs({ md::utils::generateId(), "some_name", { { "bool", false } } }),
        PlainParametrisedTestArgs({ md::utils::generateId(), "entity", { { "int", 34 }, { "float", 34 } } }),
        PlainParametrisedTestArgs({ md::utils::generateId(), "", { { "string", "str123" } } })));

TEST_P(PlainParametrisedTest, testParameters)
{
    PlainParametrisedTestArgs args = GetParam();
    PlainParametrised entity(GetParam().name, GetParam().id, GetParam().params);

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

TEST_P(PlainParametrisedTest, testConstructFromMap)
{
    PlainParametrisedTestArgs args = GetParam();

    QVariantMap map;
    map.insert(props::id, args.id.toString());
    map.insert(props::name, args.name);
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    PlainParametrised entity(map);

    EXPECT_EQ(entity.id, args.id);
    EXPECT_EQ(entity.name, args.name);
    EXPECT_EQ(entity.parameters(), args.params);
}

TEST_P(PlainParametrisedTest, testFromVariant)
{
    PlainParametrisedTestArgs args = GetParam();
    PlainParametrised entity(QString(), args.id);

    QVariantMap map;
    map.insert(props::name, args.name);
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    entity.fromVariantMap(map);
    EXPECT_EQ(entity.id, args.id);
    EXPECT_EQ(entity.name, args.name);
    EXPECT_EQ(entity.parameters(), args.params);
}

TEST_P(PlainParametrisedTest, testToVariant)
{
    PlainParametrisedTestArgs args = GetParam();
    PlainParametrised entity(GetParam().name, GetParam().id, GetParam().params);

    QVariantMap map;
    map.insert(props::id, args.id);
    map.insert(props::name, args.name);
    map.insert(props::params, args.params);

    EXPECT_EQ(map, entity.toVariantMap());
}
