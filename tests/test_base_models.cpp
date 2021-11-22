#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "kjarni_traits.h"
#include "parametrised.h"

using namespace md::domain;

struct BaseModelsTestArgs
{
    QVariant id;
    QString name;
    QVariantMap params;
};

class BaseModelsTest : public ::testing::TestWithParam<BaseModelsTestArgs>
{
public:
    BaseModelsTest() = default;
};

INSTANTIATE_TEST_SUITE_P(
    instantiation, BaseModelsTest,
    ::testing::Values(
        BaseModelsTestArgs({ md::utils::generateId(), "Name 665", {} }),
        BaseModelsTestArgs({ md::utils::generateId(), "some_name", { { "bool_propery", false } } }),
        BaseModelsTestArgs({ md::utils::generateId(),
                             "entity",
                             { { "int_propery", 34 }, { "float_propery", 34 } } }),
        BaseModelsTestArgs({ md::utils::generateId(), "", { { "string_propery", "str123" } } })));

TEST_P(BaseModelsTest, testParameters)
{
    BaseModelsTestArgs args = GetParam();
    Parametrised entity(GetParam().id, GetParam().name, GetParam().params);

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

TEST_P(BaseModelsTest, testConstructFromMap)
{
    BaseModelsTestArgs args = GetParam();

    QVariantMap map;
    map.insert(props::id, args.id.toString());
    map.insert(props::name, args.name);
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    Parametrised entity(map);

    EXPECT_EQ(entity.id(), args.id);
    EXPECT_EQ(entity.name(), args.name);
    EXPECT_EQ(entity.parameters(), args.params);
}

TEST_P(BaseModelsTest, testFromVariant)
{
    BaseModelsTestArgs args = GetParam();
    Parametrised entity(args.id, QString());

    QVariantMap map;
    map.insert(props::name, args.name);
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    entity.fromVariantMap(map);
    EXPECT_EQ(entity.id(), args.id);
    EXPECT_EQ(entity.name(), args.name);
    EXPECT_EQ(entity.parameters(), args.params);
}

TEST_P(BaseModelsTest, testToVariant)
{
    BaseModelsTestArgs args = GetParam();
    Parametrised entity(GetParam().id, GetParam().name, GetParam().params);

    QVariantMap map;
    map.insert(props::id, args.id);
    map.insert(props::name, args.name);
    map.insert(props::params, args.params);

    EXPECT_EQ(map, entity.toVariantMap());
}
