#include <gtest/gtest.h>

#include <QSignalSpy>

#include "route.h"

using namespace md::domain;

class RouteTest : public ::testing::Test
{
public:
    RouteTest() : route()
    {
    }

    Route route;
};

TEST_F(RouteTest, testRouteAddWaypoints)
{
    QSignalSpy addSpy(&route, &Route::waypointAdded);

    Waypoint wpt1("WPT 1", QString());
    Waypoint wpt2("WPT 2", QString());

    route.addWaypoint(&wpt1);

    EXPECT_EQ(addSpy.count(), 1);
    EXPECT_EQ(route.count(), 1);
    EXPECT_TRUE(route.waypoints().contains(&wpt1));
    EXPECT_FALSE(route.waypoints().contains(&wpt2));

    route.addWaypoint(&wpt2);

    EXPECT_EQ(addSpy.count(), 2);
    EXPECT_EQ(route.count(), 2);
    EXPECT_TRUE(route.waypoints().contains(&wpt1));
    EXPECT_TRUE(route.waypoints().contains(&wpt2));
}

TEST_F(RouteTest, testRouteRemoveWaypoints)
{
    QSignalSpy removeSpy(&route, &Route::waypointRemoved);

    WaypointType type("WPT", {});

    Waypoint wpt1("WPT 1", QString());
    Waypoint wpt2("WPT 2", QString());

    route.addWaypoint(&wpt1);
    route.addWaypoint(&wpt2);

    route.removeWaypoint(&wpt2);

    EXPECT_EQ(removeSpy.count(), 1);
    EXPECT_EQ(route.count(), 1);
    EXPECT_TRUE(route.waypoints().contains(&wpt1));
    EXPECT_FALSE(route.waypoints().contains(&wpt2));

    route.removeWaypoint(&wpt1);

    EXPECT_EQ(removeSpy.count(), 2);
    EXPECT_EQ(route.count(), 0);
    EXPECT_FALSE(route.waypoints().contains(&wpt1));
    EXPECT_FALSE(route.waypoints().contains(&wpt2));
}
