#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "test_mission_traits.h"
#include "typed_parametrised.h"
#include "utils.h"

using namespace md::domain;

struct TypedParametrisedTestArgs
{
    QVariant id;
    QVariantMap paramValues;
    QVector<const ParameterType*> paramTypes;
};

class TypedParametrisedTest : public ::testing::TestWithParam<TypedParametrisedTestArgs>
{
public:
    TypedParametrisedTest() = default;
};

INSTANTIATE_TEST_SUITE_P(
    instantiation, TypedParametrisedTest,
    ::testing::Values(
        TypedParametrisedTestArgs({ md::utils::generateId(),
                                    { {} },
                                    test_mission::changeAltitude.parameters.values().toVector() }),
        TypedParametrisedTestArgs({ md::utils::generateId(),
                                    { { test_mission::passthrough.id, false } },
                                    test_mission::waypoint.parameters.values().toVector() }),
        TypedParametrisedTestArgs({ md::utils::generateId(),
                                    { { test_mission::radius.id, 650 } },
                                    test_mission::circle.parameters.values().toVector() }),
        TypedParametrisedTestArgs({ md::utils::generateId(),
                                    { { test_mission::radius.id, 350 } },
                                    test_mission::loop.parameters.values().toVector() })));

TEST_P(TypedParametrisedTest, testConstructWithoutParameters)
{
    TypedParametrisedTestArgs args = GetParam();
    TypedParametrisedMixin<Entity> entity(args.paramTypes, args.id);

    for (const ParameterType* type : qAsConst(args.paramTypes))
    {
        TypedParameter* parameter = entity.parameter(type->id);
        ASSERT_TRUE(parameter);

        EXPECT_EQ(parameter->value(), type->defaultValue);
    }
}

TEST_P(TypedParametrisedTest, testConstructWithParameters)
{
    TypedParametrisedTestArgs args = GetParam();
    TypedParametrisedMixin<Entity> entity(args.paramTypes, args.paramValues, args.id);

    for (const ParameterType* type : qAsConst(args.paramTypes))
    {
        TypedParameter* parameter = entity.parameter(type->id);
        ASSERT_TRUE(parameter);

        if (args.paramValues.contains(type->id))
            EXPECT_EQ(parameter->value(), args.paramValues.value(type->id));
        else
            EXPECT_EQ(parameter->value(), type->defaultValue);
    }
}

TEST_P(TypedParametrisedTest, testFromVariantMap)
{
    TypedParametrisedTestArgs args = GetParam();
    TypedParametrisedMixin<Entity> entity(args.paramTypes, args.id);

    QVariantMap map;
    map.insert(props::params, args.paramValues);
    entity.fromVariantMap(map);

    for (const ParameterType* type : qAsConst(args.paramTypes))
    {
        TypedParameter* parameter = entity.parameter(type->id);
        ASSERT_TRUE(parameter);

        if (args.paramValues.contains(type->id))
            EXPECT_EQ(parameter->value(), args.paramValues.value(type->id));
        else
            EXPECT_EQ(parameter->value(), type->defaultValue);
    }
}

TEST_P(TypedParametrisedTest, testToVariantMap)
{
    TypedParametrisedTestArgs args = GetParam();
    TypedParametrisedMixin<Entity> entity(args.paramTypes, args.paramValues, args.id);

    QVariantMap map = entity.toVariantMap();
    EXPECT_EQ(map.value(props::id), args.id);

    QVariantMap parameters = map.value(props::params).toMap();
    for (const ParameterType* type : qAsConst(args.paramTypes))
    {
        if (args.paramValues.contains(type->id))
            EXPECT_EQ(parameters.value(type->id), args.paramValues.value(type->id));
        else
            EXPECT_EQ(parameters.value(type->id), type->defaultValue);
    }
}

//TEST_P(TypedParametrisedTest, testGuardParams)
//{
//    QSignalSpy spyChanged(&entity, &Entity::changed);
//    EXPECT_EQ(spyChanged.count(), 1);

//}

TEST_P(TypedParametrisedTest, testResetParamsToDefaultValues)
{
    TypedParametrisedTestArgs args = GetParam();
    TypedParametrisedMixin<Entity> entity(args.paramTypes, args.paramValues, args.id);

    int changes = 1;
    for (const ParameterType* type : qAsConst(args.paramTypes))
    {
        TypedParameter* parameter = entity.parameter(type->id);
        ASSERT_TRUE(parameter);

        parameter->reset();
        EXPECT_EQ(parameter->value(), type->defaultValue);
    }
}

// TODO:
//TEST_P(TypedParametrisedTest, testResetParamsByType)
//{
//    TypedParametrisedTestArgs args = GetParam();
//    TypedParametrisedMixin<Entity> entity(args.paramTypes, args.paramValues, args.id);

//    QVector<const ParameterType*> waypointTypes = test_mission::waypoint.parameters.values()
//                                                      .toVector();
//    entity.resetTypeParameters(waypointTypes);

//    for (TypedParameter* parameter : entity.parameters())
//    {
//        if (args.paramValues.contains(parameter->id) || !args.paramTypes.contains(parameter->type()))
//            EXPECT_EQ(parameter->value(), args.paramValues.value(parameter->id));
//        else
//            EXPECT_EQ(parameter->value(), parameter->type()->defaultValue);
//    }
//}
