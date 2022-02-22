#include <gtest/gtest.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

#include "parameter.h"

using namespace md::domain;

struct ParameterTestArgs
{
    Parameter::Type type;
    QVariant defaultValue;
    QVariant minValue;
    QVariant maxValue;
};

class ParameterTest : public ::testing::TestWithParam<ParameterTestArgs>
{
public:
    ParameterTest()
    {
    }
};

INSTANTIATE_TEST_SUITE_P(instantiation, ParameterTest,
                         ::testing::Values(ParameterTestArgs({ Parameter::Int, 0, 0, 255 }),
                                           ParameterTestArgs({ Parameter::Int, -1, -10, 10 }),
                                           ParameterTestArgs({ Parameter::Real, -3.14, -3.14,
                                                               100 })));

TEST_P(ParameterTest, testConstruct)
{
    ParameterTestArgs args = GetParam();
    Parameter parameter("parameter", "Parameter", args.type, args.defaultValue, args.minValue,
                        args.maxValue);

    EXPECT_EQ(parameter.type(), args.type);
    EXPECT_EQ(parameter.value(), args.defaultValue);
    EXPECT_EQ(parameter.defaultValue(), args.defaultValue);
    EXPECT_EQ(parameter.minValue(), args.minValue);
    EXPECT_EQ(parameter.maxValue(), args.maxValue);
}

TEST_P(ParameterTest, testToVariant)
{
    ParameterTestArgs args = GetParam();
    Parameter parameter("parameter", "Parameter", args.type, args.defaultValue, args.minValue,
                        args.maxValue);

    QVariantMap map = parameter.toVariantMap();
    EXPECT_EQ(map.value(props::id), parameter.id());
    EXPECT_EQ(map.value(props::name), parameter.name());
    EXPECT_EQ(map.value(props::value), parameter.value());
    EXPECT_EQ(map.value(props::defaultValue), parameter.defaultValue());
    EXPECT_EQ(map.value(props::minValue), parameter.minValue());
    EXPECT_EQ(map.value(props::maxValue), parameter.maxValue());
}

TEST_P(ParameterTest, testSetAndCheck)
{
    ParameterTestArgs args = GetParam();
    Parameter parameter("parameter", "Parameter", args.type, args.defaultValue, args.minValue,
                        args.maxValue);
    QSignalSpy spyChanged(&parameter, &Parameter::changed);

    // Check mid between min and max
    QVariant mid = (args.minValue.toReal() + args.maxValue.toReal()) / 2;
    parameter.setAndCheckValue(mid);
    EXPECT_EQ(parameter.value(), mid);
    EXPECT_EQ(spyChanged.count(), 1);

    // Check less than min
    parameter.setAndCheckValue(args.minValue.toReal() - 1);
    EXPECT_EQ(parameter.value(), args.minValue);
    EXPECT_EQ(spyChanged.count(), 2);

    // Check more than max
    parameter.setAndCheckValue(args.maxValue.toReal() + 1);
    EXPECT_EQ(parameter.value(), args.maxValue);
    EXPECT_EQ(spyChanged.count(), 3);

    // Chech if the same
    parameter.setAndCheckValue(args.maxValue.toReal());
    EXPECT_EQ(parameter.value(), args.maxValue);
    EXPECT_EQ(spyChanged.count(), 3);
}
