#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QtMath>

#include "magic_property.hpp"

using namespace md;

class MagicPropertyTest : public ::testing::Test
{
public:
    MagicPropertyTest()
    {
    }
};

TEST_F(MagicPropertyTest, testConstProperty)
{
    utils::ConstProperty<int> integer(42);
    utils::ConstProperty<float> real(3.14);
    utils::ConstProperty<QVariant> variant("qwerty123");

    EXPECT_EQ(integer, 42);
    EXPECT_TRUE(qFuzzyCompare(real, static_cast<float>(3.14)));
    EXPECT_EQ(variant, QVariant("qwerty123"));
}

TEST_F(MagicPropertyTest, testProperty)
{
    int notifyCounter = 0;

    utils::Property<int> intProperty(42, [&notifyCounter]() {
        notifyCounter++;
    });

    utils::Property<QString> stringProperty("qwerty", [&notifyCounter]() {
        notifyCounter++;
    });

    intProperty = 42;
    stringProperty = "qwerty";
    EXPECT_EQ(notifyCounter, 0);

    intProperty = 665;
    stringProperty = "abc123";

    EXPECT_EQ(intProperty, 665);
    EXPECT_EQ(stringProperty, "abc123");
    EXPECT_EQ(notifyCounter, 2);
}

TEST_F(MagicPropertyTest, testRealProperty)
{
    int notifyCounter = 0;

    utils::Property<float> floatProperty(3.141592653589793, [&notifyCounter]() {
        notifyCounter++;
    });

    floatProperty = 3.141592653589793;
    EXPECT_EQ(notifyCounter, 0);

    floatProperty = 937.2101;

    EXPECT_TRUE(qFuzzyCompare(floatProperty, static_cast<float>(937.2101)));
    EXPECT_EQ(notifyCounter, 1);
}
