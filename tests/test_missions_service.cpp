#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QSignalSpy>

#include "missions_service.h"

using namespace ::testing;
using namespace md::domain;

class JsonGatewayMock : public md::data_source::IJsonGateway
{
public:
    MOCK_METHOD(QVariantList, selectIds, (), (const, override));
    MOCK_METHOD(QList<QJsonObject>, readAll, (), (const, override));
    MOCK_METHOD(QJsonObject, read, (const QVariant&), (const, override));

    MOCK_METHOD(void, save, (const QVariant&, const QJsonObject&), (override));
    MOCK_METHOD(void, remove, (const QVariant&), (override));
};

class MissionServiceTest : public Test
{
public:
    MissionServiceTest() : service(new JsonGatewayMock())
    {
    }

    MissionsService service;
};

TEST_F(MissionServiceTest, testCreateMission)
{
    QSignalSpy addSpy(&service, &MissionsService::missionAdded);
    QSignalSpy removeSpy(&service, &MissionsService::missionRemoved);

    service.createMission("test type");

    EXPECT_EQ(service.missions().count(), 1);
    EXPECT_EQ(addSpy.count(), 1);
    Mission* mission = service.missions().first();

    service.removeMission(mission);

    EXPECT_EQ(service.missions().count(), 0);
    EXPECT_FALSE(service.missions().contains(mission));
    EXPECT_EQ(removeSpy.count(), 1);
}
