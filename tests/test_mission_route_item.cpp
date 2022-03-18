#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "mission_route_item.h"
#include "test_mission_traits.h"
#include "utils.h"

using namespace md::domain;

struct MissionRouteItemTestArgs
{
    QVariant id;
    QString name;
    QVariantMap position;
    QVariantMap params;
    const MissionItemType* type;
};

class MissionRouteItemTest : public ::testing::TestWithParam<MissionRouteItemTestArgs>
{
public:
    MissionRouteItemTest() = default;
};

INSTANTIATE_TEST_SUITE_P(
    instantiation, MissionRouteItemTest,
    ::testing::Values(MissionRouteItemTestArgs({ md::utils::generateId(),
                                                 "WPT 1",
                                                 {},
                                                 { { test_mission::passthrough.id, false } },
                                                 &test_mission::waypoint }),
                      MissionRouteItemTestArgs({ md::utils::generateId(),
                                                 "WPT 2",
                                                 { { geo::latitude, 54.196783 },
                                                   { geo::longitude, -23.59275 },
                                                   { geo::altitude, 550 },
                                                   { geo::datum, geo::datums::wgs84 } },
                                                 { { test_mission::radius.id, 650 } },
                                                 &test_mission::circle }),
                      MissionRouteItemTestArgs({ md::utils::generateId(),
                                                 "WPT 3",
                                                 { { geo::latitude, -62.10348 },
                                                   { geo::longitude, 138.74623 },
                                                   { geo::altitude, 550 },
                                                   { geo::datum, geo::datums::wgs84 } },
                                                 { { test_mission::radius.id, 350 } },
                                                 &test_mission::loop })));

TEST_P(MissionRouteItemTest, testConstruct)
{
    MissionRouteItemTestArgs args = GetParam();

    MissionRouteItem item(args.type, args.name, args.id, args.params, Geodetic(args.position));

    EXPECT_EQ(item.type(), args.type);
    EXPECT_EQ(item.name(), args.name);
    EXPECT_EQ(item.id(), args.id);
    EXPECT_EQ(item.parametersMap(), args.params);
    EXPECT_EQ(item.current(), false);
    EXPECT_EQ(item.reached(), false);
}

TEST_P(MissionRouteItemTest, testConstructFromMap)
{
    MissionRouteItemTestArgs args = GetParam();
    QVariantMap map = { { props::name, args.name },
                        { props::id, args.id },
                        { props::params, args.params },
                        { props::position, args.position } };

    MissionRouteItem item(args.type, map);

    EXPECT_EQ(item.type(), args.type);
    EXPECT_EQ(item.name(), args.name);
    EXPECT_EQ(item.id(), args.id);
    EXPECT_EQ(item.parametersMap(), args.params);
    EXPECT_EQ(item.current(), false);
    EXPECT_EQ(item.reached(), false);
}

TEST_P(MissionRouteItemTest, testToVariant)
{
    MissionRouteItemTestArgs args = GetParam();

    MissionRouteItem item(args.type, args.name, args.id, args.params, Geodetic(args.position));
    QVariantMap map = item.toVariantMap();

    EXPECT_EQ(map.value(props::type), args.type->id);
    EXPECT_EQ(map.value(props::name), args.name);
    EXPECT_EQ(map.value(props::id), args.id);
    EXPECT_EQ(map.value(props::params), args.params);
    if (!args.position.isEmpty())
    {
        EXPECT_EQ(map.value(props::position), args.position);
    }
    EXPECT_EQ(map.value(props::current), false);
    EXPECT_EQ(map.value(props::reached), false);
}
