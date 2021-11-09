#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "route_item.h"
#include "test_mission_traits.h"

using namespace md::domain;

struct RouteItemTestArgs
{
    QVariant id;
    QString name;
    QVariantMap params;
    const RouteItemType* type;
};

class RouteItemTest : public ::testing::TestWithParam<RouteItemTestArgs>
{
public:
    RouteItemTest() = default;
};

INSTANTIATE_TEST_SUITE_P(
    instantiation, RouteItemTest,
    ::testing::Values(RouteItemTestArgs({ md::utils::generateId(),
                                          "WPT 1",
                                          { { route::relativeAlt.id, true },
                                            { test_mission::passthrough.id, false } },
                                          &test_mission::waypoint }),
                      RouteItemTestArgs({ md::utils::generateId(),
                                          "CH ALT 2",
                                          { { test_mission::altitude.id, 850 },
                                            { route::relativeAlt.id, true } },
                                          &test_mission::changeAltitude })));

TEST_P(RouteItemTest, testConstructFromMap)
{
    RouteItemTestArgs args = GetParam();

    QVariantMap map;
    map.insert(props::id, args.id.toString());
    map.insert(props::name, args.name);
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    RouteItem routeItem(args.type, map);

    EXPECT_EQ(routeItem.id(), args.id);
    EXPECT_EQ(routeItem.name(), args.name);
    EXPECT_EQ(routeItem.parameters(), args.params);
    EXPECT_EQ(routeItem.type(), args.type);
}

TEST_P(RouteItemTest, testFromVariant)
{
    RouteItemTestArgs args = GetParam();
    RouteItem routeItem(args.type, QString(), args.id);

    QVariantMap map;
    map.insert(props::name, args.name);
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    routeItem.fromVariantMap(map);
    EXPECT_EQ(routeItem.name(), args.name);
    EXPECT_EQ(routeItem.parameters(), args.params);
}

TEST_P(RouteItemTest, testToVariant)
{
    RouteItemTestArgs args = GetParam();
    RouteItem routeItem(args.type, args.name, args.id);
    routeItem.setParameters(args.params);

    QVariantMap map;
    map.insert(props::id, args.id);
    map.insert(props::name, args.name);
    if (!args.params.isEmpty())
        map.insert(props::params, args.params);
    map.insert(props::type, args.type->id);

    EXPECT_EQ(map, routeItem.toVariantMap());
}

TEST_P(RouteItemTest, testConstructDefaultParamsByType)
{
    RouteItemTestArgs args = GetParam();
    RouteItem routeItem(args.type, args.name, args.id);

    EXPECT_EQ(routeItem.parameters(), args.type->defaultParameters());
}

TEST_P(RouteItemTest, testResetParamToTypeDeafult)
{
    RouteItemTestArgs args = GetParam();
    RouteItem routeItem(args.type, args.name, args.id);

    if (args.params.isEmpty())
        return;

    routeItem.setParameter(args.params.firstKey(), args.params.first());

    if (routeItem.parameters() == args.type->defaultParameters())
        return;

    routeItem.resetParameter(args.params.firstKey());

    EXPECT_EQ(routeItem.parameters(), args.type->defaultParameters());
}

TEST_P(RouteItemTest, testResetParamsToTypeDeafults)
{
    RouteItemTestArgs args = GetParam();
    RouteItem routeItem(args.type, args.name, args.id);
    routeItem.setParameters(args.params);

    EXPECT_NE(routeItem.parameters(), args.type->defaultParameters());

    routeItem.resetParameters();

    EXPECT_EQ(routeItem.parameters(), args.type->defaultParameters());
}

TEST_P(RouteItemTest, testSyncParams)
{
    RouteItemTestArgs args = GetParam();
    RouteItem routeItem(args.type, args.name, args.id);
    routeItem.setParameters(args.params);

    routeItem.syncParameters();

    EXPECT_EQ(routeItem.parameters().keys(), args.type->defaultParameters().keys());
}
