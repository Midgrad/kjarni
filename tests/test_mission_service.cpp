#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QSignalSpy>

#include "missions_service.h"
#include "test_mission_traits.h"

using namespace testing;
using namespace md::domain;

class MissionsRepositoryMock : public IMissionsRepository
{
public:
    MOCK_METHOD(QVariantMap, select, (const QVariant&), (override));
    MOCK_METHOD(QVariant, selectMissionIdForVehicle, (const QVariant&), (override));
    MOCK_METHOD(QVariantList, selectMissionIds, (), (override));

    MOCK_METHOD(void, insert, (Mission*), (override));
    MOCK_METHOD(void, read, (Mission*), (override));
    MOCK_METHOD(void, update, (Mission*), (override));
    MOCK_METHOD(void, remove, (Mission*), (override));
};

class MissionRouteItemsRepositoryMock : public IMissionItemsRepository
{
public:
    MOCK_METHOD(QVariantMap, select, (const QVariant&), (override));
    MOCK_METHOD(QVariantList, selectMissionRouteItemIds, (const QVariant&), (override));

    MOCK_METHOD(void, insert, (MissionRouteItem*, const QVariant&), (override));
    MOCK_METHOD(void, read, (MissionRouteItem*), (override));
    MOCK_METHOD(void, update, (MissionRouteItem*), (override));
    MOCK_METHOD(void, remove, (MissionRouteItem*), (override));
    MOCK_METHOD(void, removeById, (const QVariant&), (override));
};

class MissionServiceTest : public Test
{
public:
    MissionsRepositoryMock missions;
    MissionRouteItemsRepositoryMock items;
    MissionsService service;

    MissionServiceTest() : service(&missions, &items)
    {
    }

    void SetUp() override
    {
        service.registerMissionType(&test_mission::missionType);
    }

    void TearDown() override
    {
        service.unregisterMissionType(&test_mission::missionType);
    }
};

TEST_F(MissionServiceTest, testReadAll)
{
    const QVariant mission1Id = "mission 1";

    const QVariant item11Id = "item 1";
    const QVariant item12Id = md::utils::generateId();

    const QVariant mission2Id = md::utils::generateId();
    const QVariant item21Id = md::utils::generateId();
    const QVariant item22Id = md::utils::generateId();

    QSignalSpy spyAdded(&service, &IMissionsService::missionAdded);

    // Select all missions id
    EXPECT_CALL(missions, selectMissionIds())
        .WillOnce(Return(QVariantList({ mission1Id, mission2Id })));

    // Select mission 1
    EXPECT_CALL(missions, select(mission1Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::missionType.id } })));
    EXPECT_CALL(items, selectMissionRouteItemIds(mission1Id))
        .WillOnce(Return(QVariantList({ item11Id, item12Id })));
    // Select item11
    EXPECT_CALL(items, select(item11Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::waypoint.id } })));
    // Select item12
    EXPECT_CALL(items, select(item12Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::circle.id } })));

    // Select mission 2
    EXPECT_CALL(missions, select(mission2Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::missionType.id } })));
    EXPECT_CALL(items, selectMissionRouteItemIds(mission2Id))
        .WillOnce(Return(QVariantList({ item21Id, item22Id })));
    // Select item21
    EXPECT_CALL(items, select(item21Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::waypoint.id } })));
    // Select item22
    EXPECT_CALL(items, select(item22Id))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::waypoint.id } })));

    service.readAll();

    EXPECT_EQ(service.missionIds().count(), 2);
    EXPECT_EQ(service.missions().count(), 2);
    EXPECT_EQ(spyAdded.count(), 2);

    Mission* mission1 = service.mission(mission1Id);
    ASSERT_TRUE(mission1);
    ASSERT_EQ(mission1->route()->count(), 2);

    Mission* mission2 = service.mission(mission2Id);
    ASSERT_TRUE(mission2);
    ASSERT_EQ(mission2->route()->count(), 2);
}

TEST_F(MissionServiceTest, testInsertMission)
{
    QSignalSpy spyAdded(&service, &IMissionsService::missionAdded);

    Mission* mission = new Mission(&test_mission::missionType, "Test mission");
    MissionRouteItem* wpt1 = new MissionRouteItem(&test_mission::waypoint, "WPT 1");
    mission->route()->addItem(wpt1);
    MissionRouteItem* wpt2 = new MissionRouteItem(&test_mission::waypoint, "WPT 2");
    mission->route()->addItem(wpt2);
    MissionRouteItem* wpt3 = new MissionRouteItem(&test_mission::changeAltitude, "CH ALT 3");
    mission->route()->addItem(wpt3);
    MissionRouteItem* wpt4 = new MissionRouteItem(&test_mission::waypoint, "WPT 4");
    mission->route()->addItem(wpt4);
    MissionRouteItem* wpt5 = new MissionRouteItem(&test_mission::changeAltitude, "CH ALT 5");
    mission->route()->addItem(wpt5);
    MissionRouteItem* wpt6 = new MissionRouteItem(&test_mission::changeSpeed, "CH SPD 6");
    mission->route()->addItem(wpt6);

    // Insert mission
    EXPECT_CALL(missions, insert(mission)).Times(1);
    EXPECT_CALL(items, selectMissionRouteItemIds(mission->id())).WillOnce(Return(QVariantList({})));

    // Insert mission items
    EXPECT_CALL(items, insert(wpt1, mission->id())).Times(1);
    EXPECT_CALL(items, insert(wpt2, mission->id())).Times(1);
    EXPECT_CALL(items, insert(wpt3, mission->id())).Times(1);
    EXPECT_CALL(items, insert(wpt4, mission->id())).Times(1);
    EXPECT_CALL(items, insert(wpt5, mission->id())).Times(1);
    EXPECT_CALL(items, insert(wpt6, mission->id())).Times(1);

    service.saveMission(mission);

    ASSERT_EQ(service.missions().count(), 1);
    EXPECT_EQ(spyAdded.count(), 1);
    EXPECT_TRUE(service.missionIds().contains(mission->id));
}

TEST_F(MissionServiceTest, testUpdateMission)
{
    // fixture
    Mission* mission = new Mission(&test_mission::missionType, "Test mission");
    MissionRouteItem* wpt1 = new MissionRouteItem(&test_mission::waypoint, "WPT 1");
    mission->route()->addItem(wpt1);
    MissionRouteItem* wpt2 = new MissionRouteItem(&test_mission::waypoint, "WPT 2");
    mission->route()->addItem(wpt2);
    MissionRouteItem* wpt3 = new MissionRouteItem(&test_mission::waypoint, "WPT 4");
    mission->route()->addItem(wpt3);

    service.addMission(mission);

    // test
    QSignalSpy spyChanged(&service, &IMissionsService::missionChanged);

    MissionRouteItem* wpt4 = new MissionRouteItem(&test_mission::waypoint, "WPT 6");
    mission->route()->removeItem(wpt1);
    mission->route()->removeItem(wpt3);
    mission->route()->addItem(wpt4);

    // Update mission
    EXPECT_CALL(missions, update(mission)).Times(1);
    EXPECT_CALL(items, selectMissionRouteItemIds(mission->id()))
        .WillOnce(Return(QVariantList({ wpt1->id, wpt2->id, wpt3->id })));

    // Update wpt2
    EXPECT_CALL(items, update(wpt2)).Times(1);
    // Insert wpt4
    EXPECT_CALL(items, insert(wpt4, mission->id())).Times(1);

    // Remove wpt1 & wpt3
    EXPECT_CALL(items, removeById(wpt1->id())).Times(1);
    EXPECT_CALL(items, removeById(wpt3->id())).Times(1);

    service.saveMission(mission);

    EXPECT_EQ(spyChanged.count(), 1);
}

TEST_F(MissionServiceTest, testRestoreMission)
{
    // fixture
    Mission* mission = new Mission(&test_mission::missionType, "Test mission");
    MissionRouteItem* wpt1 = new MissionRouteItem(&test_mission::waypoint, "WPT 1");
    mission->route()->addItem(wpt1);
    MissionRouteItem* wpt2 = new MissionRouteItem(&test_mission::waypoint, "WPT 2");
    mission->route()->addItem(wpt2);
    MissionRouteItem* wpt3 = new MissionRouteItem(&test_mission::waypoint, "WPT 4");
    mission->route()->addItem(wpt3);

    service.addMission(mission);

    // test
    QSignalSpy spyChanged(&service, &IMissionsService::missionChanged);

    mission->route()->removeItem(wpt1);
    mission->route()->removeItem(wpt3);
    MissionRouteItem* wpt4 = new MissionRouteItem(&test_mission::waypoint, "WPT 6");
    mission->route()->addItem(wpt4);

    // Read mission
    EXPECT_CALL(missions, read(mission)).Times(1);
    EXPECT_CALL(items, selectMissionRouteItemIds(mission->id()))
        .WillOnce(Return(QVariantList({ wpt1->id, wpt2->id, wpt3->id })));

    // Read wpt2
    EXPECT_CALL(items, read(wpt2)).Times(1);

    // wpt4 will just be removed form the mission

    // Select wpt1 & wpt3
    EXPECT_CALL(items, select(wpt1->id()))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::waypoint.id } })));
    EXPECT_CALL(items, select(wpt3->id()))
        .WillOnce(Return(QVariantMap({ { props::type, test_mission::waypoint.id } })));

    service.restoreMission(mission);

    EXPECT_EQ(spyChanged.count(), 1);
    EXPECT_EQ(mission->route()->count(), 3);
}

TEST_F(MissionServiceTest, testRemoveMission)
{
    // fixture
    Mission* mission = new Mission(&test_mission::missionType, "Test mission");
    MissionRouteItem* wpt1 = new MissionRouteItem(&test_mission::waypoint, "WPT 1");
    mission->route()->addItem(wpt1);
    MissionRouteItem* wpt2 = new MissionRouteItem(&test_mission::waypoint, "WPT 2");
    mission->route()->addItem(wpt2);
    MissionRouteItem* wpt3 = new MissionRouteItem(&test_mission::waypoint, "WPT 4");
    mission->route()->addItem(wpt3);

    service.addMission(mission);

    // test
    QSignalSpy spyRemove(&service, &IMissionsService::missionRemoved);

    mission->route()->removeItem(wpt1);
    mission->route()->removeItem(wpt3);
    MissionRouteItem* wpt4 = new MissionRouteItem(&test_mission::waypoint, "WPT 6");
    mission->route()->addItem(wpt4);

    // Remove mission
    EXPECT_CALL(missions, remove(mission)).Times(1);
    EXPECT_CALL(items, selectMissionRouteItemIds(mission->id()))
        .WillOnce(Return(QVariantList({ wpt1->id, wpt2->id, wpt3->id })));

    // Remove wpt1
    EXPECT_CALL(items, removeById(wpt1->id())).Times(1);

    // Remove wpt2 & wpt3
    EXPECT_CALL(items, removeById(wpt2->id())).Times(1);
    EXPECT_CALL(items, removeById(wpt3->id())).Times(1);

    service.removeMission(mission);

    EXPECT_EQ(spyRemove.count(), 1);
    EXPECT_EQ(service.missions().count(), 0);
}

TEST_F(MissionServiceTest, testUpdateMissionRouteItem)
{
    // fixture
    Mission* mission = new Mission(&test_mission::missionType, "Test mission");
    MissionRouteItem* wpt = new MissionRouteItem(&test_mission::waypoint, "WPT 1");
    mission->route()->addItem(wpt);

    service.addMission(mission);

    // Check mission
    EXPECT_CALL(items, selectMissionRouteItemIds(mission->id()))
        .WillOnce(Return(QVariantList({ wpt->id })));

    // Update wpt
    EXPECT_CALL(items, update(wpt)).Times(1);

    service.saveItem(mission->route, wpt);
}

TEST_F(MissionServiceTest, testRestoreMissionRouteItem)
{
    // fixture
    Mission* mission = new Mission(&test_mission::missionType, "Test mission");
    MissionRouteItem* wpt = new MissionRouteItem(&test_mission::waypoint, "WPT 1");
    mission->route()->addItem(wpt);

    service.addMission(mission);

    // Read wpt
    EXPECT_CALL(items, read(wpt)).Times(1);

    service.restoreItem(mission->route(), wpt);
}
