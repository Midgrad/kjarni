#include <cmath>
#include <limits>

#include <gtest/gtest.h>

#include "geodetic.h"

using namespace md::domain;

struct TestArgs
{
    double latitude;
    double longitude;
    double altitude;
    QString datum;
};

class GeodeticTest : public ::testing::TestWithParam<TestArgs>
{
public:
    GeodeticTest()
    {
    }
};

INSTANTIATE_TEST_SUITE_P(
    instantiation, GeodeticTest,
    ::testing::Values(TestArgs({ 44.5432, 31.2344, 4584.56, datums::wgs84 }),
                      TestArgs({ -18.1283, -164.8748, 202.82, datums::wgs84 }),
                      TestArgs({ 0, 0, 0, "" }),
                      TestArgs({ std::numeric_limits<double>::quiet_NaN(),
                                 std::numeric_limits<double>::quiet_NaN(), 0, "" }),
                      TestArgs({ std::numeric_limits<double>::quiet_NaN(),
                                 std::numeric_limits<double>::quiet_NaN(),
                                 std::numeric_limits<double>::quiet_NaN(), "" })));

TEST_P(GeodeticTest, testToVariant)
{
    TestArgs args = GetParam();

    Geodetic geodetic(args.latitude, args.longitude, args.altitude, args.datum);
    QVariantMap map = geodetic.toVariantMap();

    if (geodetic.isValidPosition())
    {
        EXPECT_DOUBLE_EQ(map.value(::latitude).toDouble(), args.latitude);
        EXPECT_DOUBLE_EQ(map.value(::longitude).toDouble(), args.longitude);
    }
    else
    {
        EXPECT_TRUE(std::isnan(map.value(::latitude).toDouble()));
        EXPECT_TRUE(std::isnan(map.value(::longitude).toDouble()));
    }

    if (geodetic.isValidAltitude())
    {
        EXPECT_FLOAT_EQ(map.value(::altitude).toDouble(), args.altitude);
    }
    else
    {
        EXPECT_TRUE(std::isnan(map.value(::altitude).toDouble()));
    }

    EXPECT_EQ(map.value(::datum).toString(), args.datum);
}

TEST_P(GeodeticTest, testFromVariant)
{
    TestArgs args = GetParam();

    QVariantMap map({ { ::latitude, args.latitude },
                      { ::longitude, args.longitude },
                      { ::altitude, args.altitude },
                      { ::datum, args.datum } });
    Geodetic geodetic(map);

    if (geodetic.isValidPosition())
    {
        EXPECT_DOUBLE_EQ(geodetic.latitude(), args.latitude);
        EXPECT_DOUBLE_EQ(geodetic.longitude(), args.longitude);
    }

    if (geodetic.isValidAltitude())
    {
        EXPECT_FLOAT_EQ(geodetic.altitude(), args.altitude);
    }

    EXPECT_EQ(geodetic.datum(), args.datum);
}

TEST_P(GeodeticTest, testEquality)
{
    TestArgs args = GetParam();

    Geodetic first(args.latitude, args.longitude, args.altitude, args.datum);
    Geodetic second(first.toVariantMap());

    if (first.isValidPosition())
    {
        EXPECT_DOUBLE_EQ(first.latitude(), second.latitude());
        EXPECT_DOUBLE_EQ(first.longitude(), second.longitude());
    }
    else
    {
        EXPECT_TRUE(!second.isValidPosition());
    }

    if (first.isValidAltitude())
    {
        EXPECT_FLOAT_EQ(first.altitude(), second.altitude());
    }
    else
    {
        EXPECT_TRUE(!second.isValidAltitude());
    }

    if (first.isValid() && second.isValid())
    {
        EXPECT_TRUE(first == second);
    }

    EXPECT_EQ(first.datum(), second.datum());
}
