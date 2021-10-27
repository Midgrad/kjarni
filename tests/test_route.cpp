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
        waypoints.append(QVariantMap({ { params::id, QUuid::createUuid() },
                                       { params::name, "WPT 1" },
                                       { params::type, test_mission::waypoint.name } }));
        waypoints.append(QVariantMap({ { params::id, QUuid::createUuid() },
                                       { params::name, "WPT 2" },
                                       { params::type, test_mission::waypoint.name } }));
        waypoints.append(QVariantMap({ { params::id, QUuid::createUuid() },
                                       { params::name, "CRL 3" },
                                       { params::type, test_mission::circle.name } }));
    }

    QVariantList waypoints;
};

TEST_F(RouteTest, testConstructFromMap)
{
    QVariantMap map;
    map.insert(params::id, QUuid::createUuid());
    map.insert(params::name, "Test route");
    map.insert(params::waypoints, waypoints);

    Route route(&test_mission::routeType, map);

    EXPECT_EQ(route.id(), map.value(params::id));
    EXPECT_EQ(route.name(), map.value(params::name));
    EXPECT_EQ(route.waypoints().count(), waypoints.count());

    int index = 0;
    for (const QVariant& value : qAsConst(waypoints))
    {
        QVariantMap waypoint = value.toMap();

        EXPECT_EQ(route.waypoint(index)->id(), waypoint.value(params::id));
        EXPECT_EQ(route.waypoint(index)->name(), waypoint.value(params::name));
        EXPECT_EQ(route.waypoint(index)->type()->name, waypoint.value(params::type));
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
        EXPECT_EQ(route.waypoint(index)->type()->name, waypoint.value(params::type));
        index++;
    }
}

TEST_F(RouteTest, testAddWaypoint)
{
    Route route(&test_mission::routeType, "Route");

    EXPECT_TRUE(route.waypoints().isEmpty());

    auto wpt = new Waypoint(&test_mission::waypoint, "WPT");
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
    EXPECT_FALSE(route.waypoints().contains(wpt));
    delete wpt;
}

TEST_F(RouteTest, testSetWaypoints)
{
    QVariantMap map;
    map.insert(params::name, "Test route");
    map.insert(params::waypoints, waypoints);

    Route route(&test_mission::routeType, map);

    ASSERT_EQ(route.count(), 3);

    QList<Waypoint*> newWaypoints = route.waypoints();
    newWaypoints.removeFirst();
    auto newWpt = new Waypoint(&test_mission::waypoint, "WPT");
    newWaypoints.append(newWpt);

    route.setWaypoints(newWaypoints);

    EXPECT_EQ(route.waypoints(), newWaypoints);
}
