#include <gtest/gtest.h>

#include <QSignalSpy>

#include "test_mission_traits.h"
#include "waypoint.h"

using namespace md::domain;

struct WaypointTestArgs
{
    QVariant id;
    QString name;
    bool current;
    bool reached;
    bool confirmed;
    QVariantMap params;
    const WaypointType* type;
};

class WaypointTest : public ::testing::TestWithParam<WaypointTestArgs>
{
public:
    WaypointTest() = default;
};

INSTANTIATE_TEST_SUITE_P(instantiation, WaypointTest,
                         ::testing::Values(WaypointTestArgs({ md::utils::generateId(),
                                                              "WPT 1",
                                                              true,
                                                              false,
                                                              false,
                                                              { { mission::latitude.id, 54.196783 },
                                                                { mission::longitude.id, 41.397421 },
                                                                { mission::altitude.id, 850 },
                                                                { mission::relativeAlt.id, true } },
                                                              &test_mission::waypoint }),
                                           WaypointTestArgs({ md::utils::generateId(),
                                                              "WPT 2",
                                                              false,
                                                              false,
                                                              false,
                                                              { { mission::latitude.id, 54.196783 },
                                                                { mission::longitude.id, 41.397421 },
                                                                { mission::altitude.id, 850 },
                                                                { mission::relativeAlt.id, true } },
                                                              &test_mission::waypoint }),
                                           WaypointTestArgs({ md::utils::generateId(),
                                                              "CRL 2",
                                                              true,
                                                              false,
                                                              true,
                                                              { { mission::latitude.id, 54.196783 },
                                                                { mission::longitude.id, 41.397421 },
                                                                { mission::altitude.id, 850 },
                                                                { mission::relativeAlt.id, true } },
                                                              &test_mission::circle })));

TEST_P(WaypointTest, testConstructFromMap)
{
    WaypointTestArgs args = GetParam();

    QVariantMap map;
    map.insert(params::id, args.id.toString());
    map.insert(params::name, args.name);
    map.insert(params::params, QJsonValue::fromVariant(args.params));
    map.insert(params::current, args.current);
    map.insert(params::reached, args.reached);
    map.insert(params::confirmed, args.confirmed);

    Waypoint waypoint(args.type, map);

    EXPECT_EQ(waypoint.id(), args.id);
    EXPECT_EQ(waypoint.name(), args.name);
    EXPECT_EQ(waypoint.parameters(), args.params);
    EXPECT_EQ(waypoint.type(), args.type);
    EXPECT_EQ(waypoint.current(), args.current);
    EXPECT_EQ(waypoint.reached(), args.reached);
    EXPECT_EQ(waypoint.confirmed(), args.confirmed);
}

TEST_P(WaypointTest, testFromVariant)
{
    WaypointTestArgs args = GetParam();
    Waypoint waypoint(args.type, QString(), args.id);

    QVariantMap map;
    map.insert(params::name, args.name);
    map.insert(params::params, QJsonValue::fromVariant(args.params));
    map.insert(params::current, args.current);
    map.insert(params::reached, args.reached);
    map.insert(params::confirmed, args.confirmed);

    waypoint.fromVariantMap(map);
    EXPECT_EQ(waypoint.name(), args.name);
    EXPECT_EQ(waypoint.parameters(), args.params);
    EXPECT_EQ(waypoint.current(), args.current);
    EXPECT_EQ(waypoint.reached(), args.reached);
    EXPECT_EQ(waypoint.confirmed(), args.confirmed);
}

TEST_P(WaypointTest, testToVariant)
{
    WaypointTestArgs args = GetParam();
    Waypoint waypoint(args.type, GetParam().name, GetParam().id, GetParam().params);
    waypoint.setCurrent(args.current);
    waypoint.setReached(args.reached);
    waypoint.setConfirmed(args.confirmed);

    QVariantMap map;
    map.insert(params::id, args.id);
    map.insert(params::name, args.name);
    if (!args.params.isEmpty())
        map.insert(params::params, args.params);
    map.insert(params::type, args.type->id);
    map.insert(params::current, args.current);
    map.insert(params::reached, args.reached);
    map.insert(params::confirmed, args.confirmed);

    EXPECT_EQ(map, waypoint.toVariantMap(true));
}

TEST_P(WaypointTest, testConstructDefaultParamsByType)
{
    WaypointTestArgs args = GetParam();
    Waypoint waypoint(args.type, GetParam().name, GetParam().id);

    EXPECT_EQ(waypoint.parameters(), args.type->defaultParameters());
}

TEST_P(WaypointTest, testResetParamToTypeDeafult)
{
    WaypointTestArgs args = GetParam();
    Waypoint waypoint(args.type, GetParam().name, GetParam().id);

    if (args.params.isEmpty())
        return;

    waypoint.setParameter(args.params.firstKey(), args.params.first());

    if (waypoint.parameters() == args.type->defaultParameters())
        return;

    waypoint.resetParameter(args.params.firstKey());

    EXPECT_EQ(waypoint.parameters(), args.type->defaultParameters());
}

TEST_P(WaypointTest, testResetParamsToTypeDeafults)
{
    WaypointTestArgs args = GetParam();
    Waypoint waypoint(args.type, GetParam().name, GetParam().id, GetParam().params);

    EXPECT_NE(waypoint.parameters(), args.type->defaultParameters());

    waypoint.resetParameters();

    EXPECT_EQ(waypoint.parameters(), args.type->defaultParameters());
}

TEST_P(WaypointTest, testSyncParams)
{
    WaypointTestArgs args = GetParam();
    Waypoint waypoint(args.type, GetParam().name, GetParam().id, GetParam().params);

    waypoint.syncParameters();

    EXPECT_EQ(waypoint.parameters().keys(), args.type->defaultParameters().keys());
}
