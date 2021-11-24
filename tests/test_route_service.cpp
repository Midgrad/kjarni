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
    MOCK_METHOD(void, removeByIds, (const QVariantList&), (override));
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
    const QVariant route2Id = md::utils::generateId();

    const QVariant item11Id = "item 1";
    const QVariant item12Id = md::utils::generateId();
    const QVariant item121Id = md::utils::generateId();
    const QVariant item21Id = 3;
    const QVariant item22Id = 5;

    QSignalSpy spyAdded(&service, &IRoutesService::routeAdded);

    EXPECT_CALL(routes, selectRouteIds()).WillOnce(Return(QVariantList({ route1Id, route2Id })));
    EXPECT_CALL(routes, select(route1Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::routeType.id } })));
    EXPECT_CALL(routes, select(route2Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::routeType.id } })));

    EXPECT_CALL(items, select(item11Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::waypoint.id } })));
    EXPECT_CALL(items, select(item12Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::waypoint.id } })));
    EXPECT_CALL(items, select(item121Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::changeSpeed.id } })));
    EXPECT_CALL(items, select(item21Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::waypoint.id } })));
    EXPECT_CALL(items, select(item22Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::waypoint.id } })));

    EXPECT_CALL(items, selectChildItemsIds(route1Id))
        .WillOnce(Return(QVariantList({ item11Id, item12Id })));
    EXPECT_CALL(items, selectChildItemsIds(route2Id))
        .WillOnce(Return(QVariantList({ item21Id, item22Id })));
    EXPECT_CALL(items, selectChildItemsIds(item11Id)).WillOnce(Return(QVariantList({})));
    EXPECT_CALL(items, selectChildItemsIds(item12Id)).WillOnce(Return(QVariantList({ item121Id })));
    EXPECT_CALL(items, selectChildItemsIds(item121Id)).WillOnce(Return(QVariantList({})));
    EXPECT_CALL(items, selectChildItemsIds(item21Id)).WillOnce(Return(QVariantList({})));
    EXPECT_CALL(items, selectChildItemsIds(item22Id)).WillOnce(Return(QVariantList({})));

    service.readAll();

    EXPECT_EQ(service.routeIds().count(), 2);
    EXPECT_EQ(service.routes().count(), 2);
    EXPECT_EQ(spyAdded.count(), 2);
}

TEST_F(RouteServiceTest, testSave)
{
    QSignalSpy spyAdded(&service, &IRoutesService::routeAdded);
    QSignalSpy spyChanged(&service, &IRoutesService::routeChanged);
}

TEST_F(RouteServiceTest, testRestore)
{
    QSignalSpy spyChanged(&service, &IRoutesService::routeChanged);
}

TEST_F(RouteServiceTest, testRemove)
{
    QSignalSpy spyRemove(&service, &IRoutesService::routeRemoved);
}
