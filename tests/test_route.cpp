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
        waypoints.append(QVariantMap({ { props::id, md::utils::generateId() },
                                       { props::name, "WPT 1" },
                                       { props::type, test_mission::waypoint.id } }));
        waypoints.append(QVariantMap({ { props::id, md::utils::generateId() },
                                       { props::name, "WPT 2" },
                                       { props::type, test_mission::waypoint.id } }));
        waypoints.append(QVariantMap({ { props::id, md::utils::generateId() },
                                       { props::name, "CRL 3" },
                                       { props::type, test_mission::circle.id } }));
    }

    QVariantList waypoints;
};

TEST_F(RouteTest, testConstructFromMap)
{
    QVariantMap map;
    map.insert(props::id, md::utils::generateId());
    map.insert(props::name, "Test route");
    map.insert(props::waypoints, waypoints);

    Route route(&test_mission::routeType, map);

    EXPECT_EQ(route.id(), map.value(props::id));
    EXPECT_EQ(route.name(), map.value(props::name));
    EXPECT_EQ(route.waypointsCount(), waypoints.count());

    int index = 0;
    for (const QVariant& value : qAsConst(waypoints))
    {
        QVariantMap waypoint = value.toMap();

        EXPECT_EQ(route.waypoint(index)->id(), waypoint.value(props::id));
        EXPECT_EQ(route.waypoint(index)->name(), waypoint.value(props::name));
        EXPECT_EQ(route.waypoint(index)->type()->id, waypoint.value(props::type));
        index++;
    }
}

TEST_F(RouteTest, testFromVariant)
{
    Route route(&test_mission::routeType, "Invalid name");

    QVariantMap map;
    map.insert(props::name, "Test route");
    map.insert(props::waypoints, waypoints);

    route.fromVariantMap(map);

    EXPECT_EQ(route.name(), map.value(props::name));
    EXPECT_EQ(route.waypointsCount(), waypoints.count());

    int index = 0;
    for (const QVariant& value : qAsConst(waypoints))
    {
        QVariantMap waypoint = value.toMap();

        EXPECT_EQ(route.waypoint(index)->id(), waypoint.value(props::id));
        EXPECT_EQ(route.waypoint(index)->name(), waypoint.value(props::name));
        EXPECT_EQ(route.waypoint(index)->type()->id, waypoint.value(props::type));
        index++;
    }
}

TEST_F(RouteTest, testAddWaypoint)
{
    Route route(&test_mission::routeType, "Route");

    EXPECT_TRUE(route.waypointsCount() == 0);
    EXPECT_TRUE(route.itemsCount() == 0);

    auto wpt = new Waypoint(&test_mission::waypoint);
    route.addWaypoint(wpt);

    ASSERT_TRUE(route.waypointsCount() == 1);
    EXPECT_TRUE(route.itemsCount() == 1);
    EXPECT_EQ(route.waypoint(0), wpt);
}

TEST_F(RouteTest, testRemoveWaypoint)
{
    QVariantMap map;
    map.insert(props::name, "Test route");
    map.insert(props::waypoints, waypoints);

    Route route(&test_mission::routeType, map);

    ASSERT_EQ(route.waypointsCount(), 3);

    auto wpt = route.waypoint(1);
    EXPECT_EQ(wpt->parent(), &route);
    route.removeWaypoint(wpt);

    EXPECT_EQ(route.waypointsCount(), 2);
    EXPECT_EQ(wpt->parent(), nullptr);
    EXPECT_FALSE(route.waypointIndex(wpt) > -1);
    delete wpt;
}

TEST_F(RouteTest, testSetWaypoints)
{
    QVariantMap map;
    map.insert(props::name, "Test route");
    map.insert(props::waypoints, waypoints);

    Route route(&test_mission::routeType, map);

    ASSERT_EQ(route.waypointsCount(), 3);

    QList<Waypoint*> newWaypoints = route.waypoints();
    newWaypoints.removeFirst();
    auto newWpt = new Waypoint(&test_mission::waypoint);
    newWaypoints.append(newWpt);

    route.setWaypoints(newWaypoints);

    EXPECT_EQ(route.waypoints(), newWaypoints);
}

TEST_F(RouteTest, testAdd2WaypointAndItems)
{
    Route route(&test_mission::routeType, "Route");

    EXPECT_TRUE(route.waypointsCount() == 0);
    EXPECT_TRUE(route.itemsCount() == 0);

    auto wpt1 = new Waypoint(&test_mission::waypoint);
    auto item11 = new WaypointItem(&test_mission::changeSpeed);
    wpt1->addItem(item11);
    route.addWaypoint(wpt1);

    auto wpt2 = new Waypoint(&test_mission::waypoint);
    auto item21 = new WaypointItem(&test_mission::changeSpeed);
    auto item22 = new WaypointItem(&test_mission::changeAltitude);
    wpt2->addItem(item21);
    wpt2->addItem(item22);
    route.addWaypoint(wpt2);

    ASSERT_TRUE(route.waypointsCount() == 2);
    ASSERT_TRUE(route.itemsCount() == 5);
    EXPECT_EQ(route.waypoint(0), wpt1);
    EXPECT_EQ(route.waypoint(1), wpt2);
    EXPECT_EQ(route.item(0), wpt1);
    EXPECT_EQ(route.item(1), item11);
    EXPECT_EQ(route.item(2), wpt2);
    EXPECT_EQ(route.item(3), item21);
    EXPECT_EQ(route.item(4), item22);
}
