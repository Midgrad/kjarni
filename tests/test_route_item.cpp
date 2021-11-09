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
    bool current;
    bool reached;
    bool confirmed;
    QVariantMap params;
    const RouteItemType* type;
};

class RouteItemTest : public ::testing::TestWithParam<RouteItemTestArgs>
{
public:
    RouteItemTest() = default;

    bool compareMapsWithNaNs(const QVariantMap& first, const QVariantMap& second)
    {
        if (first.count() != second.count())
            return false;

        for (auto it = first.begin(); it != first.end(); ++it)
        {
            if (qIsNaN(it.value().toDouble()) && qIsNaN(second.value(it.key()).toDouble()))
                continue;

            if (it.value() != second.value(it.key()))
            {
                return false;
            }
        }
        return true;
    }
};

INSTANTIATE_TEST_SUITE_P(instantiation, RouteItemTest,
                         ::testing::Values(RouteItemTestArgs({ md::utils::generateId(),
                                                               "WPT 1",
                                                               true,
                                                               false,
                                                               false,
                                                               { { route::latitude.id, 54.196783 },
                                                                 { route::longitude.id, 41.397421 },
                                                                 { route::altitude.id, 850 },
                                                                 { route::relativeAlt.id, true } },
                                                               &test_mission::waypoint }),
                                           RouteItemTestArgs({ md::utils::generateId(),
                                                               "WPT 2",
                                                               false,
                                                               false,
                                                               false,
                                                               { { route::latitude.id, 54.196783 },
                                                                 { route::longitude.id, 41.397421 },
                                                                 { route::altitude.id, 850 },
                                                                 { route::relativeAlt.id, true } },
                                                               &test_mission::waypoint }),
                                           RouteItemTestArgs({ md::utils::generateId(),
                                                               "CRL 2",
                                                               true,
                                                               false,
                                                               true,
                                                               { { route::latitude.id, 54.196783 },
                                                                 { route::longitude.id, 41.397421 },
                                                                 { route::altitude.id, 850 },
                                                                 { route::relativeAlt.id, true } },
                                                               &test_mission::circle })));

TEST_P(RouteItemTest, testConstructFromMap)
{
    RouteItemTestArgs args = GetParam();

    QVariantMap map;
    map.insert(params::id, args.id.toString());
    map.insert(params::name, args.name);
    map.insert(params::params, QJsonValue::fromVariant(args.params));
    map.insert(params::current, args.current);
    map.insert(params::reached, args.reached);
    map.insert(params::confirmed, args.confirmed);

    RouteItem waypoint(args.type, map);

    EXPECT_EQ(waypoint.id(), args.id);
    EXPECT_EQ(waypoint.name(), args.name);
    EXPECT_EQ(waypoint.parameters(), args.params);
    EXPECT_EQ(waypoint.type(), args.type);
    EXPECT_EQ(waypoint.current(), args.current);
    EXPECT_EQ(waypoint.reached(), args.reached);
    EXPECT_EQ(waypoint.confirmed(), args.confirmed);
}

TEST_P(RouteItemTest, testFromVariant)
{
    RouteItemTestArgs args = GetParam();
    RouteItem waypoint(args.type, QString(), args.id);

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

TEST_P(RouteItemTest, testToVariant)
{
    RouteItemTestArgs args = GetParam();
    RouteItem waypoint(args.type, GetParam().name, GetParam().id, GetParam().params);
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

    EXPECT_EQ(map, waypoint.toVariantMap());
}

TEST_P(RouteItemTest, testConstructDefaultParamsByType)
{
    RouteItemTestArgs args = GetParam();
    RouteItem waypoint(args.type, GetParam().name, GetParam().id);

    EXPECT_TRUE(compareMapsWithNaNs(waypoint.parameters(), args.type->defaultParameters()));
}

TEST_P(RouteItemTest, testResetParamToTypeDeafult)
{
    RouteItemTestArgs args = GetParam();
    RouteItem waypoint(args.type, GetParam().name, GetParam().id);

    if (args.params.isEmpty())
        return;

    waypoint.setParameter(args.params.firstKey(), args.params.first());

    if (waypoint.parameters() == args.type->defaultParameters())
        return;

    waypoint.resetParameter(args.params.firstKey());

    EXPECT_TRUE(compareMapsWithNaNs(waypoint.parameters(), args.type->defaultParameters()));
}

TEST_P(RouteItemTest, testResetParamsToTypeDeafults)
{
    RouteItemTestArgs args = GetParam();
    RouteItem waypoint(args.type, GetParam().name, GetParam().id, GetParam().params);

    EXPECT_NE(waypoint.parameters(), args.type->defaultParameters());

    waypoint.resetParameters();

    EXPECT_TRUE(compareMapsWithNaNs(waypoint.parameters(), args.type->defaultParameters()));
}

TEST_P(RouteItemTest, testSyncParams)
{
    RouteItemTestArgs args = GetParam();
    RouteItem waypoint(args.type, GetParam().name, GetParam().id, GetParam().params);

    waypoint.syncParameters();

    EXPECT_EQ(waypoint.parameters().keys(), args.type->defaultParameters().keys());
}
