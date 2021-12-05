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
        items.append(QVariantMap({ { props::id, md::utils::generateId() },
                                   { props::name, "WPT 1" },
                                   { props::type, test_mission::waypoint.id } }));
        items.append(QVariantMap({ { props::id, md::utils::generateId() },
                                   { props::name, "WPT 2" },
                                   { props::type, test_mission::waypoint.id } }));
        items.append(QVariantMap({ { props::id, md::utils::generateId() },
                                   { props::name, "CRL 3" },
                                   { props::type, test_mission::circle.id } }));
    }

    QVariantList items;
};

TEST_F(RouteTest, testConstructFromMap)
{
    QVariantMap map;
    map.insert(props::id, md::utils::generateId());
    map.insert(props::name, "Test route");

    Route route(&test_mission::routeType, map);

    EXPECT_EQ(route.id, map.value(props::id));
    EXPECT_EQ(route.name, map.value(props::name).toString());
}

TEST_F(RouteTest, testFromVariant)
{
    Route route(&test_mission::routeType, "Invalid name");

    QVariantMap map;
    map.insert(props::name, "Test route");

    route.fromVariantMap(map);

    EXPECT_EQ(route.name, map.value(props::name).toString());
}

TEST_F(RouteTest, testAddWaypoint)
{
    Route route(&test_mission::routeType, "Route");

    EXPECT_TRUE(route.count() == 0);
    EXPECT_TRUE(route.totalCount() == 0);

    auto wpt = new RouteItem(&test_mission::waypoint);
    route.addItem(wpt);

    ASSERT_TRUE(route.count() == 1);
    EXPECT_TRUE(route.totalCount() == 1);
    EXPECT_EQ(route.item(0), wpt);
}

TEST_F(RouteTest, testRemoveWaypoint)
{
    QVariantMap map;
    map.insert(props::name, "Test route");

    Route route(&test_mission::routeType, map);
    for (const QVariant& itemData : items)
    {
        QVariantMap item = itemData.toMap();
        route.addItem(
            new RouteItem(test_mission::routeType.itemType(item.value(props::type).toString()),
                          item));
    }

    ASSERT_EQ(route.count(), 3);

    auto wpt = route.item(1);
    EXPECT_EQ(wpt->parent(), &route);
    route.removeItem(wpt);

    EXPECT_EQ(route.count(), 2);
    EXPECT_EQ(wpt->parent(), nullptr);
    EXPECT_FALSE(route.index(wpt) > -1);
}

TEST_F(RouteTest, testSetWaypoints)
{
    QVariantMap map;
    map.insert(props::name, "Test route");
    Route route(&test_mission::routeType, map);

    ASSERT_EQ(route.count(), 0);

    QList<RouteItem*> newWaypoints;
    auto newWpt = new RouteItem(&test_mission::waypoint);
    newWaypoints.append(newWpt);

    route.setItems(newWaypoints);

    EXPECT_EQ(route.items(), newWaypoints);
}

TEST_F(RouteTest, testAdd2WaypointAndItems)
{
    Route route(&test_mission::routeType, "Route");

    EXPECT_TRUE(route.count() == 0);
    EXPECT_TRUE(route.totalCount() == 0);

    auto wpt1 = new RouteItem(&test_mission::waypoint);
    auto item11 = new RouteItem(&test_mission::changeSpeed);
    wpt1->addItem(item11);
    route.addItem(wpt1);

    auto wpt2 = new RouteItem(&test_mission::waypoint);
    auto item21 = new RouteItem(&test_mission::changeSpeed);
    auto item22 = new RouteItem(&test_mission::changeAltitude);
    wpt2->addItem(item21);
    wpt2->addItem(item22);
    route.addItem(wpt2);

    ASSERT_TRUE(route.count() == 2);
    ASSERT_TRUE(route.totalCount() == 5);
    EXPECT_EQ(route.item(0), wpt1);
    EXPECT_EQ(route.item(1), wpt2);
    EXPECT_EQ(route.itemByFlatIndex(0), wpt1);
    EXPECT_EQ(route.itemByFlatIndex(1), item11);
    EXPECT_EQ(route.itemByFlatIndex(2), wpt2);
    EXPECT_EQ(route.itemByFlatIndex(3), item21);
    EXPECT_EQ(route.itemByFlatIndex(4), item22);
}
