#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "route_item.h"
#include "test_mission_traits.h"
#include "utils.h"

using namespace md::domain;

struct RouteItemTestArgs
{
    QVariant id;
    QVariantMap position;
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
                                          {},
                                          { { test_mission::passthrough.id, false } },
                                          &test_mission::waypoint }),
                      RouteItemTestArgs({ md::utils::generateId(),
                                          { { geo::latitude, 54.196783 },
                                            { geo::longitude, -23.59275 },
                                            { geo::altitude, 550 } },
                                          { { test_mission::radius.id, 650 } },
                                          &test_mission::circle }),
                      RouteItemTestArgs({ md::utils::generateId(),
                                          { { geo::latitude, -62.10348 },
                                            { geo::longitude, 138.74623 },
                                            { geo::altitude, 550 } },
                                          { { test_mission::radius.id, 350 } },
                                          &test_mission::loop })));

TEST_P(RouteItemTest, testConstructFromMap)
{
    RouteItemTestArgs args = GetParam();

    QVariantMap map;
    map.insert(props::id, args.id.toString());
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    RouteItem item(args.type, map);

    EXPECT_EQ(item.id, args.id);
    EXPECT_EQ(item.parameters(), args.params);
    EXPECT_EQ(item.type(), args.type);
}

TEST_P(RouteItemTest, testFromVariant)
{
    RouteItemTestArgs args = GetParam();
    RouteItem item(args.type, args.id);

    QVariantMap map;
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    item.fromVariantMap(map);
    EXPECT_EQ(item.parameters(), args.params);
}

TEST_P(RouteItemTest, testToVariant)
{
    RouteItemTestArgs args = GetParam();

    RouteItem item(args.type, args.id);
    item.setParameters(args.params);

    QVariantMap result = item.toVariantMap();

    EXPECT_EQ(result.value(props::id), args.id);
    EXPECT_EQ(result.value(props::type), args.type->id);
    EXPECT_EQ(result.value(props::type), args.type->id);
    EXPECT_EQ(result.value(props::params), args.params);
}

TEST_P(RouteItemTest, testConstructDefaultParamsByType)
{
    RouteItemTestArgs args = GetParam();
    RouteItem itemItem(args.type, args.id);

    auto wptParams = itemItem.parameters();
    auto defaultParams = args.type->defaultParameters();
    for (auto it = wptParams.begin(); it != wptParams.end(); ++it)
    {
        if (qIsNaN(it.value().toReal()) && qIsNaN(defaultParams.value(it.key()).toReal()))
            continue;

        EXPECT_EQ(it.value(), defaultParams.value(it.key()).toReal());
    }
}

TEST_P(RouteItemTest, testResetParamToTypeDeafult)
{
    RouteItemTestArgs args = GetParam();
    RouteItem itemItem(args.type, args.id);

    if (args.params.isEmpty())
        return;

    itemItem.setParameter(args.params.firstKey(), args.params.first());

    if (itemItem.parameters() == args.type->defaultParameters())
        return;

    itemItem.resetParameter(args.params.firstKey());

    auto wptParams = itemItem.parameters();
    auto defaultParams = args.type->defaultParameters();
    for (auto it = wptParams.begin(); it != wptParams.end(); ++it)
    {
        if (qIsNaN(it.value().toReal()) && qIsNaN(defaultParams.value(it.key()).toReal()))
            continue;

        EXPECT_EQ(it.value(), defaultParams.value(it.key()).toReal());
    }
}

TEST_P(RouteItemTest, testResetParamsToTypeDeafults)
{
    RouteItemTestArgs args = GetParam();
    RouteItem itemItem(args.type, args.id);
    itemItem.setParameters(args.params);

    EXPECT_NE(itemItem.parameters(), args.type->defaultParameters());

    itemItem.resetParameters();

    auto wptParams = itemItem.parameters();
    auto defaultParams = args.type->defaultParameters();
    for (auto it = wptParams.begin(); it != wptParams.end(); ++it)
    {
        if (qIsNaN(it.value().toReal()) && qIsNaN(defaultParams.value(it.key()).toReal()))
            continue;

        EXPECT_EQ(it.value(), defaultParams.value(it.key()).toReal());
    }
}

TEST_P(RouteItemTest, testSyncParams)
{
    RouteItemTestArgs args = GetParam();
    RouteItem itemItem(args.type, args.id);
    itemItem.setParameters(args.params);

    itemItem.syncParameters();

    EXPECT_EQ(itemItem.parameters().keys(), args.type->defaultParameters().keys());
}
