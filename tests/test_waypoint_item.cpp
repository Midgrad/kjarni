#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "test_mission_traits.h"
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
                                             { { route::relativeAlt.id, true },
                                               { test_mission::passthrough.id, false } },
                                             &test_mission::waypoint }),
                      WaypointItemTestArgs({ md::utils::generateId(),
                                             { { test_mission::altitude.id, 850 },
                                               { route::relativeAlt.id, true } },
                                             &test_mission::changeAltitude })));

TEST_P(WaypointItemTest, testConstructFromMap)
{
    WaypointItemTestArgs args = GetParam();

    QVariantMap map;
    map.insert(props::id, args.id.toString());
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    WaypointItem waypointItem(args.type, map);

    EXPECT_EQ(waypointItem.id(), args.id);
    EXPECT_EQ(waypointItem.parameters(), args.params);
    EXPECT_EQ(waypointItem.type(), args.type);
}

TEST_P(WaypointItemTest, testFromVariant)
{
    WaypointItemTestArgs args = GetParam();
    WaypointItem waypointItem(args.type, args.id);

    QVariantMap map;
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    waypointItem.fromVariantMap(map);
    EXPECT_EQ(waypointItem.parameters(), args.params);
}

TEST_P(WaypointItemTest, testToVariant)
{
    WaypointItemTestArgs args = GetParam();
    WaypointItem waypointItem(args.type, args.id);
    waypointItem.setParameters(args.params);

    QVariantMap map;
    map.insert(props::id, args.id);
    map.insert(props::name, args.type->shortName);
    if (!args.params.isEmpty())
        map.insert(props::params, args.params);
    map.insert(props::type, args.type->id);

    EXPECT_EQ(map, waypointItem.toVariantMap());
}

TEST_P(WaypointItemTest, testConstructDefaultParamsByType)
{
    WaypointItemTestArgs args = GetParam();
    WaypointItem waypointItem(args.type, args.id);

    EXPECT_EQ(waypointItem.parameters(), args.type->defaultParameters());
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

    EXPECT_EQ(waypointItem.parameters(), args.type->defaultParameters());
}

TEST_P(WaypointItemTest, testResetParamsToTypeDeafults)
{
    WaypointItemTestArgs args = GetParam();
    WaypointItem waypointItem(args.type, args.id);
    waypointItem.setParameters(args.params);

    EXPECT_NE(waypointItem.parameters(), args.type->defaultParameters());

    waypointItem.resetParameters();

    EXPECT_EQ(waypointItem.parameters(), args.type->defaultParameters());
}

TEST_P(WaypointItemTest, testSyncParams)
{
    WaypointItemTestArgs args = GetParam();
    WaypointItem waypointItem(args.type, args.id);
    waypointItem.setParameters(args.params);

    waypointItem.syncParameters();

    EXPECT_EQ(waypointItem.parameters().keys(), args.type->defaultParameters().keys());
}
