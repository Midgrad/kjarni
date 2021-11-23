#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "test_mission_traits.h"
#include "utils.h"
#include "waypoint_item.h"

using namespace md::domain;

struct WaypointItemTestArgs
{
    QVariant id;
    QVariantMap params;
    const WaypointItemType* type;
};

class WaypointItemTest : public ::testing::TestWithParam<WaypointItemTestArgs>
{
public:
    WaypointItemTest() = default;
};

INSTANTIATE_TEST_SUITE_P(
    instantiation, WaypointItemTest,
    ::testing::Values(WaypointItemTestArgs({ md::utils::generateId(),
                                             { { route::relativeAlt.id, false },
                                               { test_mission::passthrough.id, true } },
                                             &test_mission::waypoint }),
                      WaypointItemTestArgs({ md::utils::generateId(),
                                             { { route::latitude.id, 54.196783 },
                                               { route::longitude.id, -23.59275 },
                                               { route::altitude.id, 550 },
                                               { route::relativeAlt.id, true },
                                               { test_mission::radius.id, 650 } },
                                             &test_mission::circle }),
                      WaypointItemTestArgs({ md::utils::generateId(),
                                             { { route::latitude.id, -62.10348 },
                                               { route::longitude.id, 138.74623 },
                                               { route::altitude.id, 550 },
                                               { route::relativeAlt.id, true },
                                               { test_mission::radius.id, 350 } },
                                             &test_mission::loop })));

TEST_P(WaypointItemTest, testConstructFromMap)
{
    WaypointItemTestArgs args = GetParam();

    QVariantMap map;
    map.insert(props::id, args.id.toString());
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    WaypointItem waypoint(args.type, map);

    EXPECT_EQ(waypoint.id(), args.id);
    EXPECT_EQ(waypoint.parameters(), args.params);
    EXPECT_EQ(waypoint.type(), args.type);
}

TEST_P(WaypointItemTest, testFromVariant)
{
    WaypointItemTestArgs args = GetParam();
    WaypointItem waypoint(args.type, args.id);

    QVariantMap map;
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    waypoint.fromVariantMap(map);
    EXPECT_EQ(waypoint.parameters(), args.params);
}

TEST_P(WaypointItemTest, testToVariant)
{
    WaypointItemTestArgs args = GetParam();

    WaypointItem waypoint(args.type, args.id);
    waypoint.setParameters(args.params);

    QVariantMap result = waypoint.toVariantMap();

    EXPECT_EQ(result.value(props::id), args.id);
    EXPECT_EQ(result.value(props::type), args.type->id);
    EXPECT_EQ(result.value(props::type), args.type->id);
    EXPECT_EQ(result.value(props::params), args.params);
}

TEST_P(WaypointItemTest, testConstructDefaultParamsByType)
{
    WaypointItemTestArgs args = GetParam();
    WaypointItem waypointItem(args.type, args.id);

    auto wptParams = waypointItem.parameters();
    auto defaultParams = args.type->defaultParameters();
    for (auto it = wptParams.begin(); it != wptParams.end(); ++it)
    {
        if (qIsNaN(it.value().toReal()) && qIsNaN(defaultParams.value(it.key()).toReal()))
            continue;

        EXPECT_EQ(it.value(), defaultParams.value(it.key()).toReal());
    }
}

TEST_P(WaypointItemTest, testResetParamToTypeDeafult)
{
    WaypointItemTestArgs args = GetParam();
    WaypointItem waypointItem(args.type, args.id);

    if (args.params.isEmpty())
        return;

    waypointItem.setParameter(args.params.firstKey(), args.params.first());

    if (waypointItem.parameters() == args.type->defaultParameters())
        return;

    waypointItem.resetParameter(args.params.firstKey());

    auto wptParams = waypointItem.parameters();
    auto defaultParams = args.type->defaultParameters();
    for (auto it = wptParams.begin(); it != wptParams.end(); ++it)
    {
        if (qIsNaN(it.value().toReal()) && qIsNaN(defaultParams.value(it.key()).toReal()))
            continue;

        EXPECT_EQ(it.value(), defaultParams.value(it.key()).toReal());
    }
}

TEST_P(WaypointItemTest, testResetParamsToTypeDeafults)
{
    WaypointItemTestArgs args = GetParam();
    WaypointItem waypointItem(args.type, args.id);
    waypointItem.setParameters(args.params);

    EXPECT_NE(waypointItem.parameters(), args.type->defaultParameters());

    waypointItem.resetParameters();

    auto wptParams = waypointItem.parameters();
    auto defaultParams = args.type->defaultParameters();
    for (auto it = wptParams.begin(); it != wptParams.end(); ++it)
    {
        if (qIsNaN(it.value().toReal()) && qIsNaN(defaultParams.value(it.key()).toReal()))
            continue;

        EXPECT_EQ(it.value(), defaultParams.value(it.key()).toReal());
    }
}

TEST_P(WaypointItemTest, testSyncParams)
{
    WaypointItemTestArgs args = GetParam();
    WaypointItem waypointItem(args.type, args.id);
    waypointItem.setParameters(args.params);

    waypointItem.syncParameters();

    EXPECT_EQ(waypointItem.parameters().keys(), args.type->defaultParameters().keys());
}
