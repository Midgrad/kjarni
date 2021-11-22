#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "test_mission_traits.h"
#include "utils.h"
#include "waypoint.h"

using namespace md::domain;

struct WaypointTestArgs
{
    QVariant id;
    Geodetic position;
    QVariantMap params;
    const WaypointItemType* type;
};

class WaypointTest : public ::testing::TestWithParam<WaypointTestArgs>
{
public:
    WaypointTest() = default;
};

INSTANTIATE_TEST_SUITE_P(
    instantiation, WaypointTest,
    ::testing::Values(
        WaypointTestArgs({ md::utils::generateId(),
                           Geodetic(),
                           { { route::relativeAlt.id, false },
                             { test_mission::passthrough.id, true } },
                           &test_mission::waypoint }),
        WaypointTestArgs({ md::utils::generateId(),
                           Geodetic(54.196783, 41.397421, 850),
                           { { route::relativeAlt.id, true }, { test_mission::radius.id, 650 } },
                           &test_mission::circle }),
        WaypointTestArgs({ md::utils::generateId(),
                           Geodetic(44.782442, 36.120283, 451.65),
                           { { route::relativeAlt.id, true }, { test_mission::radius.id, 350 } },
                           &test_mission::loop })));

TEST_P(WaypointTest, testConstructFromMap)
{
    WaypointTestArgs args = GetParam();

    QVariantMap map;
    map.insert(props::id, args.id.toString());
    md::utils::mergeMap(map, args.position.toVariantMap());
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    Waypoint waypoint(args.type, map);

    EXPECT_EQ(waypoint.id(), args.id);

    if (waypoint.position().isValid() && args.position.isValid())
        EXPECT_EQ(waypoint.position(), args.position);
    EXPECT_EQ(waypoint.parameters(), args.params);
    EXPECT_EQ(waypoint.type(), args.type);
}

TEST_P(WaypointTest, testFromVariant)
{
    WaypointTestArgs args = GetParam();
    Waypoint waypoint(args.type, args.id);

    QVariantMap map;
    md::utils::mergeMap(map, args.position.toVariantMap());
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    waypoint.fromVariantMap(map);
    if (waypoint.position().isValid() && args.position.isValid())
        EXPECT_EQ(waypoint.position(), args.position);
    EXPECT_EQ(waypoint.parameters(), args.params);
}

TEST_P(WaypointTest, testToVariant)
{
    WaypointTestArgs args = GetParam();

    Waypoint waypoint(args.type, args.id);
    waypoint.setPosition(args.position);
    waypoint.setParameters(args.params);

    QVariantMap result = waypoint.toVariantMap();

    EXPECT_EQ(result.value(props::id), args.id);
    EXPECT_EQ(result.value(props::type), args.type->id);
    EXPECT_EQ(result.value(props::type), args.type->id);
    EXPECT_EQ(result.value(props::params), args.params);

    if (args.position.isValid())
    {
        EXPECT_EQ(result.value(geo::latitude), args.position.latitude());
        EXPECT_EQ(result.value(geo::longitude), args.position.longitude());
        EXPECT_EQ(result.value(geo::altitude), args.position.altitude());
        EXPECT_EQ(result.value(geo::datum), args.position.datum());
    }
}
