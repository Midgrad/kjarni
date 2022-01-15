#include <cmath>
#include <limits>

#include <gtest/gtest.h>

#include <QDebug>

#include "cartesian.h"
#include "geo_traits.h"

using namespace md::domain;

struct CartesianTestArgs
{
    double x;
    double y;
    float z;
};

class CartesianTest : public ::testing::TestWithParam<CartesianTestArgs>
{
public:
    CartesianTest()
    {
    }
};

INSTANTIATE_TEST_SUITE_P(
    instantiation, CartesianTest,
    ::testing::Values(CartesianTestArgs({ 44.5432, 31.2344, 4584.56 }),
                      CartesianTestArgs({ -2343218.1283, -1333264.8748, 203222.82 }),
                      CartesianTestArgs({ 0, 0, 0 }),
                      CartesianTestArgs({ std::numeric_limits<double>::quiet_NaN(),
                                          std::numeric_limits<double>::quiet_NaN(), 0 }),
                      CartesianTestArgs({ std::numeric_limits<double>::quiet_NaN(),
                                          std::numeric_limits<double>::quiet_NaN(),
                                          std::numeric_limits<double>::quiet_NaN() })));

TEST_P(CartesianTest, testToVariant)
{
    CartesianTestArgs args = GetParam();

    Cartesian cartesian(args.x, args.y, args.z);
    QVariantMap map = cartesian.toVariantMap();

    if (cartesian.isValid())
    {
        EXPECT_DOUBLE_EQ(map.value(geo::x).toDouble(), args.x);
        EXPECT_DOUBLE_EQ(map.value(geo::y).toDouble(), args.y);
        EXPECT_DOUBLE_EQ(map.value(geo::z).toDouble(), args.z);
    }
    else
    {
        EXPECT_TRUE(std::isnan(map.value(geo::x).toDouble()) ||
                    std::isnan(map.value(geo::y).toDouble()) ||
                    std::isnan(map.value(geo::z).toDouble()));
    }
}

TEST_P(CartesianTest, testFromVariant)
{
    CartesianTestArgs args = GetParam();

    QVariantMap map({ { geo::x, args.x }, { geo::y, args.y }, { geo::z, args.z } });
    Cartesian cartesian(map);

    if (cartesian.isValid())
    {
        EXPECT_DOUBLE_EQ(cartesian.x(), args.x);
        EXPECT_DOUBLE_EQ(cartesian.y(), args.y);
        EXPECT_DOUBLE_EQ(cartesian.z(), args.z);
    }
}

TEST_P(CartesianTest, testTranslated)
{
    CartesianTestArgs args = GetParam();

    Cartesian cartesian(args.x, args.y, args.z);
    Cartesian diff(10, -50.2, 50.02);

    Cartesian result = cartesian.translated(diff).translated(-diff);

    if (cartesian.isValid())
        EXPECT_TRUE(cartesian == result);
    else
        EXPECT_FALSE(result.isValid());
}

TEST_P(CartesianTest, testEquality)
{
    CartesianTestArgs args = GetParam();

    Cartesian first(args.x, args.y, args.z);
    Cartesian second(first.toVariantMap());

    if (first.isValid())
    {
        EXPECT_DOUBLE_EQ(first.x(), second.x());
        EXPECT_DOUBLE_EQ(first.y(), second.y());
        EXPECT_DOUBLE_EQ(first.z(), second.z());
    }
    else
    {
        EXPECT_TRUE(!second.isValid());
    }

    if (first.isValid() && second.isValid())
    {
        EXPECT_TRUE(first == second);
    }
}
