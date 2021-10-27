#include <gtest/gtest.h>

#include <QSignalSpy>

#include "test_mission_traits.h"
#include "waypoint.h"

using namespace md::domain;

struct WaypointTestArgs
{
    QUuid id;
    QString name;
    QVariantMap params;
    const WaypointType* type;
};

class WaypointTest : public ::testing::TestWithParam<WaypointTestArgs>
{
public:
    WaypointTest() = default;
};

INSTANTIATE_TEST_SUITE_P(
    instantiation, WaypointTest,
    ::testing::Values(WaypointTestArgs({ QUuid::createUuid(),
                                         "WPT 1",
                                         { { mission::latitude.name, 54.196783 },
                                           { mission::longitude.name, 41.397421 },
                                           { mission::altitude.name, 850 },
                                           { mission::relative.name, true } },
                                         &test_mission::waypoint }),
                      WaypointTestArgs({ QUuid::createUuid(),
                                         "WPT 2",
                                         { { mission::latitude.name, 54.196783 },
                                           { mission::longitude.name, 41.397421 },
                                           { mission::altitude.name, 850 },
                                           { mission::relative.name, true } },
                                         &test_mission::waypoint }),
                      WaypointTestArgs({ QUuid::createUuid(),
                                         "CRL 2",
                                         { { mission::latitude.name, 54.196783 },
                                           { mission::longitude.name, 41.397421 },
                                           { mission::altitude.name, 850 },
                                           { mission::relative.name, true } },
                                         &test_mission::circle })));

TEST_P(WaypointTest, testConstructFromMap)
{
    WaypointTestArgs args = GetParam();

    QVariantMap map;
    map.insert(params::id, args.id.toString());
    map.insert(params::name, args.name);
    map.insert(params::params, QJsonValue::fromVariant(args.params));

    Waypoint waypoint(args.type, map);

    EXPECT_EQ(waypoint.id(), args.id);
    EXPECT_EQ(waypoint.name(), args.name);
    EXPECT_EQ(waypoint.parameters(), args.params);
    EXPECT_EQ(waypoint.type(), args.type);
}

TEST_P(WaypointTest, testFromVariant)
{
    WaypointTestArgs args = GetParam();
    Waypoint waypoint(args.type, QString(), args.id);

    QVariantMap map;
    map.insert(params::name, args.name);
    map.insert(params::params, QJsonValue::fromVariant(args.params));

    waypoint.fromVariantMap(map);
    EXPECT_EQ(waypoint.name(), args.name);
    EXPECT_EQ(waypoint.parameters(), args.params);
}

TEST_P(WaypointTest, testToVariant)
{
    WaypointTestArgs args = GetParam();
    Waypoint waypoint(args.type, GetParam().name, GetParam().id, GetParam().params);

    QVariantMap map;
    map.insert(params::id, args.id);
    map.insert(params::name, args.name);
    if (!args.params.isEmpty())
        map.insert(params::params, args.params);
    map.insert(params::type, args.type->name);

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
