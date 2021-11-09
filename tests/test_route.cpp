#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "route.h"
#include "test_mission_traits.h"

using namespace md::domain;

class RouteTest : public ::testing::Test
{
public:
    RouteTest()
    {
        waypoints.append(QVariantMap({ { params::id, md::utils::generateId() },
                                       { params::name, "WPT 1" },
                                       { params::type, test_mission::waypoint.id } }));
        waypoints.append(QVariantMap({ { params::id, md::utils::generateId() },
                                       { params::name, "WPT 2" },
                                       { params::type, test_mission::waypoint.id } }));
        waypoints.append(QVariantMap({ { params::id, md::utils::generateId() },
                                       { params::name, "CRL 3" },
                                       { params::type, test_mission::circle.id } }));
    }

    QVariantList waypoints;
};

TEST_F(RouteTest, testConstructFromMap)
{
    QVariantMap map;
    map.insert(params::id, md::utils::generateId());
    map.insert(params::name, "Test route");
    map.insert(params::waypoints, waypoints);

    Route route(&test_mission::routeType, map);

    EXPECT_EQ(route.id(), map.value(params::id));
    EXPECT_EQ(route.name(), map.value(params::name));
    EXPECT_EQ(route.count(), waypoints.count());

    int index = 0;
    for (const QVariant& value : qAsConst(waypoints))
    {
        QVariantMap waypoint = value.toMap();

        EXPECT_EQ(route.waypoint(index)->id(), waypoint.value(params::id));
        EXPECT_EQ(route.waypoint(index)->name(), waypoint.value(params::name));
        EXPECT_EQ(route.waypoint(index)->type()->id, waypoint.value(params::type));
        index++;
    }
}

TEST_F(RouteTest, testFromVariant)
{
    Route route(&test_mission::routeType, "Invalid name");

    QVariantMap map;
    map.insert(params::name, "Test route");
    map.insert(params::waypoints, waypoints);

    route.fromVariantMap(map);

    EXPECT_EQ(route.name(), map.value(params::name));
    EXPECT_EQ(route.count(), waypoints.count());

    int index = 0;
    for (const QVariant& value : qAsConst(waypoints))
    {
        QVariantMap waypoint = value.toMap();

        EXPECT_EQ(route.waypoint(index)->id(), waypoint.value(params::id));
        EXPECT_EQ(route.waypoint(index)->name(), waypoint.value(params::name));
        EXPECT_EQ(route.waypoint(index)->type()->id, waypoint.value(params::type));
        index++;
    }
}

TEST_F(RouteTest, testAddWaypoint)
{
    Route route(&test_mission::routeType, "Route");

    EXPECT_TRUE(route.count() == 0);

    auto wpt = new RouteItem(&test_mission::waypoint, "WPT");
    route.addWaypoint(wpt);

    ASSERT_TRUE(route.count() == 1);
    EXPECT_EQ(route.waypoint(0), wpt);
}

TEST_F(RouteTest, testRemoveWaypoint)
{
    QVariantMap map;
    map.insert(params::name, "Test route");
    map.insert(params::waypoints, waypoints);

    Route route(&test_mission::routeType, map);

    ASSERT_EQ(route.count(), 3);

    auto wpt = route.waypoint(1);
    EXPECT_EQ(wpt->parent(), &route);
    route.removeWaypoint(wpt);

    EXPECT_EQ(route.count(), 2);
    EXPECT_EQ(wpt->parent(), nullptr);
    EXPECT_FALSE(route.index(wpt) > -1);
    delete wpt;
}

TEST_F(RouteTest, testSetWaypoints)
{
    QVariantMap map;
    map.insert(params::name, "Test route");
    map.insert(params::waypoints, waypoints);

    Route route(&test_mission::routeType, map);

    ASSERT_EQ(route.count(), 3);

    QList<RouteItem*> newWaypoints = route.waypoints();
    newWaypoints.removeFirst();
    auto newWpt = new RouteItem(&test_mission::waypoint, "WPT");
    newWaypoints.append(newWpt);

    route.setWaypoints(newWaypoints);

    EXPECT_EQ(route.waypoints(), newWaypoints);
}
