#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QSignalSpy>

#include "routes_service.h"
#include "test_mission_traits.h"

using namespace testing;
using namespace md::domain;

class RoutesRepositoryMock : public IRoutesRepository
{
public:
    MOCK_METHOD(QVariantMap, select, (const QVariant&), (override));
    MOCK_METHOD(QVariantList, selectRouteIds, (), (override));

    MOCK_METHOD(void, insert, (Route*), (override));
    MOCK_METHOD(void, read, (Route*), (override));
    MOCK_METHOD(void, update, (Route*), (override));
    MOCK_METHOD(void, remove, (Route*), (override));
};

class RouteItemsRepositoryMock : public IRouteItemsRepository
{
public:
    MOCK_METHOD(QVariantMap, select, (const QVariant&), (override));
    MOCK_METHOD(QVariantList, selectChildItemsIds, (const QVariant&), (override));

    MOCK_METHOD(void, insert, (RouteItem*, const QVariant&), (override));
    MOCK_METHOD(void, read, (RouteItem*), (override));
    MOCK_METHOD(void, update, (RouteItem*, const QVariant&), (override));
    MOCK_METHOD(void, remove, (RouteItem*), (override));
    MOCK_METHOD(void, removeById, (const QVariant&), (override));
};

class RouteServiceTest : public Test
{
public:
    RoutesRepositoryMock routes;
    RouteItemsRepositoryMock items;
    RoutesService service;

    RouteServiceTest() : service(&routes, &items)
    {
    }

    void SetUp() override
    {
        service.registerRouteType(&test_mission::routeType);
    }

    void TearDown() override
    {
        service.unregisterRouteType(&test_mission::routeType);
    }
};

TEST_F(RouteServiceTest, testReadAll)
{
    const QVariant route1Id = "route 1";

    const QVariant item11Id = "item 1";
    const QVariant item12Id = md::utils::generateId();
    const QVariant item121Id = md::utils::generateId();

    const QVariant route2Id = md::utils::generateId();
    const QVariant item21Id = 3;
    const QVariant item22Id = 5;

    QSignalSpy spyAdded(&service, &IRoutesService::routeAdded);

    // Select all routes id
    EXPECT_CALL(routes, selectRouteIds()).WillOnce(Return(QVariantList({ route1Id, route2Id })));

    // Select route 1
    EXPECT_CALL(routes, select(route1Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::routeType.id } })));
    EXPECT_CALL(items, selectChildItemsIds(route1Id))
        .WillOnce(Return(QVariantList({ item11Id, item12Id })));
    // Select item11
    EXPECT_CALL(items, select(item11Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::waypoint.id } })));
    EXPECT_CALL(items, selectChildItemsIds(item11Id)).WillOnce(Return(QVariantList({})));
    // Select item12
    EXPECT_CALL(items, select(item12Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::circle.id } })));
    EXPECT_CALL(items, selectChildItemsIds(item12Id)).WillOnce(Return(QVariantList({ item121Id })));
    // Select item121
    EXPECT_CALL(items, select(item121Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::changeSpeed.id } })));
    EXPECT_CALL(items, selectChildItemsIds(item121Id)).WillOnce(Return(QVariantList({})));

    // Select route 2
    EXPECT_CALL(routes, select(route2Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::routeType.id } })));
    EXPECT_CALL(items, selectChildItemsIds(route2Id))
        .WillOnce(Return(QVariantList({ item21Id, item22Id })));
    // Select item21
    EXPECT_CALL(items, select(item21Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::waypoint.id } })));
    EXPECT_CALL(items, selectChildItemsIds(item21Id)).WillOnce(Return(QVariantList({})));
    // Select item22
    EXPECT_CALL(items, select(item22Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::waypoint.id } })));
    EXPECT_CALL(items, selectChildItemsIds(item22Id)).WillOnce(Return(QVariantList({})));

    service.readAll();

    EXPECT_EQ(service.routeIds().count(), 2);
    EXPECT_EQ(service.routes().count(), 2);
    EXPECT_EQ(spyAdded.count(), 2);

    Route* route1 = service.route(route1Id);
    ASSERT_TRUE(route1);
    ASSERT_EQ(route1->count(), 2);
    EXPECT_EQ(route1->item(0)->count(), 0);
    ASSERT_EQ(route1->item(1)->count(), 1);

    Route* route2 = service.route(route2Id);
    ASSERT_TRUE(route2);
    ASSERT_EQ(route2->count(), 2);
    EXPECT_EQ(route2->item(0)->count(), 0);
    ASSERT_EQ(route2->item(1)->count(), 0);
}

TEST_F(RouteServiceTest, testInsertRoute)
{
    QSignalSpy spyAdded(&service, &IRoutesService::routeAdded);

    Route* route = new Route(&test_mission::routeType, "Test route");
    RouteItem* wpt1 = new RouteItem(&test_mission::waypoint, "WPT 1");
    route->addItem(wpt1);
    RouteItem* wpt2 = new RouteItem(&test_mission::waypoint, "WPT 2");
    route->addItem(wpt2);
    RouteItem* pld21 = new RouteItem(&test_mission::changeAltitude, "CH ALT 3");
    wpt2->addItem(pld21);
    RouteItem* wpt3 = new RouteItem(&test_mission::waypoint, "WPT 4");
    route->addItem(wpt3);
    RouteItem* pld31 = new RouteItem(&test_mission::changeAltitude, "CH ALT 5");
    wpt3->addItem(pld31);
    RouteItem* pld32 = new RouteItem(&test_mission::changeSpeed, "CH SPD 6");
    wpt3->addItem(pld32);

    // Insert route
    EXPECT_CALL(routes, insert(route)).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(route->id())).WillOnce(Return(QVariantList({})));

    // Insert wpt1
    EXPECT_CALL(items, insert(wpt1, route->id())).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(wpt1->id())).WillOnce(Return(QVariantList({})));

    // Insert wpt2
    EXPECT_CALL(items, insert(wpt2, route->id())).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(wpt2->id())).WillOnce(Return(QVariantList({})));
    // Insert pld21
    EXPECT_CALL(items, insert(pld21, wpt2->id())).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(pld21->id())).WillOnce(Return(QVariantList({})));

    // Insert wpt3
    EXPECT_CALL(items, insert(wpt3, route->id())).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(wpt3->id())).WillOnce(Return(QVariantList({})));
    // Insert pld31
    EXPECT_CALL(items, insert(pld31, wpt3->id())).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(pld31->id())).WillOnce(Return(QVariantList({})));
    // Insert pld32
    EXPECT_CALL(items, insert(pld32, wpt3->id())).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(pld32->id())).WillOnce(Return(QVariantList({})));

    service.saveRoute(route);

    ASSERT_EQ(service.routes().count(), 1);
    EXPECT_EQ(spyAdded.count(), 1);
    EXPECT_TRUE(service.routeIds().contains(route->id()));
}

TEST_F(RouteServiceTest, testUpdateRoute)
{
    // fixture
    Route* route = new Route(&test_mission::routeType, "Test route");
    RouteItem* wpt1 = new RouteItem(&test_mission::waypoint, "WPT 1");
    route->addItem(wpt1);
    RouteItem* wpt2 = new RouteItem(&test_mission::waypoint, "WPT 2");
    route->addItem(wpt2);
    RouteItem* pld21 = new RouteItem(&test_mission::changeAltitude, "CH ALT 3");
    wpt2->addItem(pld21);
    RouteItem* wpt3 = new RouteItem(&test_mission::waypoint, "WPT 4");
    route->addItem(wpt3);
    RouteItem* pld31 = new RouteItem(&test_mission::changeSpeed, "CH SPD 5");
    wpt3->addItem(pld31);

    service.addRoute(route);

    // test
    QSignalSpy spyChanged(&service, &IRoutesService::routeChanged);

    RouteItem* wpt4 = new RouteItem(&test_mission::waypoint, "WPT 6");
    route->removeItem(wpt1);
    wpt2->removeItem(pld21);
    route->removeItem(wpt3);
    route->addItem(wpt4);

    // Update route
    EXPECT_CALL(routes, update(route)).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(route->id()))
        .WillOnce(Return(QVariantList({ wpt1->id(), wpt2->id(), wpt3->id() })));

    // Update wpt2
    EXPECT_CALL(items, update(wpt2, route->id())).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(wpt2->id()))
        .WillOnce(Return(QVariantList({ pld21->id() })));
    // Remove pld21
    EXPECT_CALL(items, selectChildItemsIds(pld21->id())).WillRepeatedly(Return(QVariantList({})));
    EXPECT_CALL(items, removeById(pld21->id())).Times(1);

    // Insert wpt4
    EXPECT_CALL(items, insert(wpt4, route->id())).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(wpt4->id())).WillRepeatedly(Return(QVariantList({})));

    // Remove wpt1
    EXPECT_CALL(items, selectChildItemsIds(wpt1->id())).WillRepeatedly(Return(QVariantList({})));
    EXPECT_CALL(items, removeById(wpt1->id())).Times(1);

    // Remove wpt3
    EXPECT_CALL(items, selectChildItemsIds(wpt3->id()))
        .WillRepeatedly(Return(QVariantList({ pld31->id() })));
    EXPECT_CALL(items, removeById(wpt3->id())).Times(1);
    // Remove pld31
    EXPECT_CALL(items, selectChildItemsIds(pld31->id())).WillRepeatedly(Return(QVariantList({})));
    EXPECT_CALL(items, removeById(pld31->id())).Times(1);

    service.saveRoute(route);

    EXPECT_EQ(spyChanged.count(), 1);
}

TEST_F(RouteServiceTest, testRestoreRoute)
{
    // fixture
    Route* route = new Route(&test_mission::routeType, "Test route");
    RouteItem* wpt1 = new RouteItem(&test_mission::waypoint, "WPT 1");
    route->addItem(wpt1);
    RouteItem* wpt2 = new RouteItem(&test_mission::waypoint, "WPT 2");
    route->addItem(wpt2);
    RouteItem* pld21 = new RouteItem(&test_mission::changeAltitude, "CH ALT 3");
    wpt2->addItem(pld21);
    RouteItem* wpt3 = new RouteItem(&test_mission::waypoint, "WPT 4");
    route->addItem(wpt3);
    RouteItem* pld31 = new RouteItem(&test_mission::changeSpeed, "CH SPD 5");
    wpt3->addItem(pld31);

    service.addRoute(route);

    // test
    QSignalSpy spyChanged(&service, &IRoutesService::routeChanged);

    route->removeItem(wpt1);
    wpt2->removeItem(pld21);
    route->removeItem(wpt3);
    RouteItem* wpt4 = new RouteItem(&test_mission::waypoint, "WPT 6");
    route->addItem(wpt4);

    // Read route
    EXPECT_CALL(routes, read(route)).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(route->id()))
        .WillOnce(Return(QVariantList({ wpt1->id(), wpt2->id(), wpt3->id() })));

    // Read wpt2
    EXPECT_CALL(items, read(wpt2)).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(wpt2->id()))
        .WillOnce(Return(QVariantList({ pld21->id() })));
    // Select pld21
    EXPECT_CALL(items, selectChildItemsIds(pld21->id())).WillRepeatedly(Return(QVariantList({})));
    EXPECT_CALL(items, select(pld21->id()))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::changeAltitude.id } })));

    // wpt4 will just be removed form the route

    // Select wpt1
    EXPECT_CALL(items, selectChildItemsIds(wpt1->id())).WillRepeatedly(Return(QVariantList({})));
    EXPECT_CALL(items, select(wpt1->id()))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::waypoint.id } })));

    // Select wpt3
    EXPECT_CALL(items, selectChildItemsIds(wpt3->id()))
        .WillRepeatedly(Return(QVariantList({ pld31->id() })));
    EXPECT_CALL(items, select(wpt3->id()))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::waypoint.id } })));
    // Select pld31
    EXPECT_CALL(items, selectChildItemsIds(pld31->id())).WillRepeatedly(Return(QVariantList({})));
    EXPECT_CALL(items, select(pld31->id()))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::changeSpeed.id } })));

    service.restoreRoute(route);

    EXPECT_EQ(spyChanged.count(), 1);
    EXPECT_EQ(route->count(), 3);
    EXPECT_EQ(wpt2->count(), 1);
}

TEST_F(RouteServiceTest, testRemoveRoute)
{
    // fixture
    Route* route = new Route(&test_mission::routeType, "Test route");
    RouteItem* wpt1 = new RouteItem(&test_mission::waypoint, "WPT 1");
    route->addItem(wpt1);
    RouteItem* wpt2 = new RouteItem(&test_mission::waypoint, "WPT 2");
    route->addItem(wpt2);
    RouteItem* pld21 = new RouteItem(&test_mission::changeAltitude, "CH ALT 3");
    wpt2->addItem(pld21);
    RouteItem* wpt3 = new RouteItem(&test_mission::waypoint, "WPT 4");
    route->addItem(wpt3);
    RouteItem* pld31 = new RouteItem(&test_mission::changeSpeed, "CH SPD 5");
    wpt3->addItem(pld31);

    service.addRoute(route);

    // test
    QSignalSpy spyRemove(&service, &IRoutesService::routeRemoved);

    route->removeItem(wpt1);
    wpt2->removeItem(pld21);
    route->removeItem(wpt3);
    RouteItem* wpt4 = new RouteItem(&test_mission::waypoint, "WPT 6");
    route->addItem(wpt4);

    // Remove route
    EXPECT_CALL(routes, remove(route)).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(route->id()))
        .WillOnce(Return(QVariantList({ wpt1->id(), wpt2->id(), wpt3->id() })));

    // Remove wpt1
    EXPECT_CALL(items, selectChildItemsIds(wpt1->id())).WillRepeatedly(Return(QVariantList({})));
    EXPECT_CALL(items, removeById(wpt1->id())).Times(1);
    // Remove wpt2
    EXPECT_CALL(items, selectChildItemsIds(wpt2->id())).WillRepeatedly(Return(QVariantList({})));
    EXPECT_CALL(items, removeById(wpt2->id())).Times(1);
    // Remove wpt3
    EXPECT_CALL(items, selectChildItemsIds(wpt3->id()))
        .WillRepeatedly(Return(QVariantList({ pld31->id() })));
    EXPECT_CALL(items, removeById(wpt3->id())).Times(1);
    // Remove pld31
    EXPECT_CALL(items, selectChildItemsIds(pld31->id())).WillRepeatedly(Return(QVariantList({})));
    EXPECT_CALL(items, removeById(pld31->id())).Times(1);

    service.removeRoute(route);

    EXPECT_EQ(spyRemove.count(), 1);
    EXPECT_EQ(service.routes().count(), 0);
}

TEST_F(RouteServiceTest, testUpdateRouteItem)
{
    // fixture
    Route* route = new Route(&test_mission::routeType, "Test route");
    RouteItem* wpt = new RouteItem(&test_mission::waypoint, "WPT 1");
    route->addItem(wpt);
    RouteItem* pld1 = new RouteItem(&test_mission::changeAltitude, "CH ALT");
    wpt->addItem(pld1);

    service.addRoute(route);

    // test
    QSignalSpy spyChanged(&service, &IRoutesService::routeChanged);

    wpt->removeItem(pld1);
    RouteItem* pld2 = new RouteItem(&test_mission::changeSpeed, "CH SPD");
    wpt->addItem(pld2);

    // Check route
    EXPECT_CALL(items, selectChildItemsIds(route->id()))
        .WillOnce(Return(QVariantList({ wpt->id() })));

    // Update wpt
    EXPECT_CALL(items, update(wpt, route->id())).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(wpt->id())).WillOnce(Return(QVariantList({ pld1->id() })));

    // Remove pld1
    EXPECT_CALL(items, selectChildItemsIds(pld1->id())).WillRepeatedly(Return(QVariantList({})));
    EXPECT_CALL(items, removeById(pld1->id())).Times(1);

    // Insert pld2
    EXPECT_CALL(items, insert(pld2, wpt->id())).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(pld2->id())).WillOnce(Return(QVariantList({})));

    service.saveItem(route, wpt);

    EXPECT_EQ(spyChanged.count(), 1);
}

TEST_F(RouteServiceTest, testRestoreRouteItem)
{
    // fixture
    Route* route = new Route(&test_mission::routeType, "Test route");
    RouteItem* wpt = new RouteItem(&test_mission::waypoint, "WPT 1");
    route->addItem(wpt);
    RouteItem* pld1 = new RouteItem(&test_mission::changeAltitude, "CH ALT");
    wpt->addItem(pld1);

    service.addRoute(route);

    // test
    QSignalSpy spyChanged(&service, &IRoutesService::routeChanged);

    wpt->removeItem(pld1);
    RouteItem* pld2 = new RouteItem(&test_mission::changeSpeed, "CH SPD");
    wpt->addItem(pld2);

    // Read wpt
    EXPECT_CALL(items, read(wpt)).Times(1);
    EXPECT_CALL(items, selectChildItemsIds(wpt->id())).WillOnce(Return(QVariantList({ pld1->id() })));

    // Select pld1
    EXPECT_CALL(items, selectChildItemsIds(pld1->id())).WillRepeatedly(Return(QVariantList({})));
    EXPECT_CALL(items, select(pld1->id()))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::changeSpeed.id } })));

    // pld2 will just be removed form the wpt

    service.restoreItem(route, wpt);

    EXPECT_EQ(spyChanged.count(), 1);
}
