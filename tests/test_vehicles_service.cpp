#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QSignalSpy>

#include "vehicle_traits.h"
#include "vehicles_service.h"

using namespace testing;
using namespace md::domain;

class VehiclesRepositoryMock : public IVehiclesRepository
{
public:
    MOCK_METHOD(QVariantList, selectVehicleIds, (), (override));
    MOCK_METHOD(QVariantMap, select, (const QVariant&), (override));

    MOCK_METHOD(void, insert, (Vehicle*), (override));
    MOCK_METHOD(void, read, (Vehicle*), (override));
    MOCK_METHOD(void, update, (Vehicle*), (override));
    MOCK_METHOD(void, remove, (Vehicle*), (override));
};

class VehiclesServiceTest : public Test
{
public:
    VehiclesRepositoryMock vehicles;
    VehiclesService service;

    VehiclesServiceTest() : service(&vehicles)
    {
        service.addVehicleType(&vehicle::generic);
    }
};

TEST_F(VehiclesServiceTest, testReadAll)
{
    QSignalSpy spyAdded(&service, &IVehiclesService::vehicleAdded);

    EXPECT_CALL(vehicles, selectVehicleIds()).WillOnce(Return(QVariantList({ "vehicle 1", 2345 })));
    EXPECT_CALL(vehicles, select(_))
        .WillRepeatedly(Return(QVariantMap({ { props::type, vehicle::generic.id } })));

    service.readAll();

    EXPECT_EQ(service.vehicleIds().count(), 2);
    EXPECT_EQ(service.vehicles().count(), 2);
    EXPECT_EQ(spyAdded.count(), 2);
}

TEST_F(VehiclesServiceTest, testSave)
{
    QSignalSpy spyAdded(&service, &IVehiclesService::vehicleAdded);
    QSignalSpy spyChanged(&service, &IVehiclesService::vehicleChanged);

    auto vehicle = new Vehicle(&vehicle::generic, "Some vehicle");

    EXPECT_CALL(vehicles, insert(vehicle)).Times(1);
    service.saveVehicle(vehicle);
    EXPECT_EQ(spyAdded.count(), 1);
    EXPECT_EQ(spyChanged.count(), 0);

    EXPECT_CALL(vehicles, update(vehicle)).Times(1);
    service.saveVehicle(vehicle);
    EXPECT_EQ(spyAdded.count(), 1);
    EXPECT_EQ(spyChanged.count(), 1);
}

TEST_F(VehiclesServiceTest, testRestore)
{
    QSignalSpy spyChanged(&service, &IVehiclesService::vehicleChanged);

    auto vehicle = new Vehicle(&vehicle::generic, "Some vehicle");

    EXPECT_CALL(vehicles, read(vehicle)).Times(1);
    service.restoreVehicle(vehicle);
    EXPECT_EQ(spyChanged.count(), 1);
}

TEST_F(VehiclesServiceTest, testRemove)
{
    QSignalSpy spyRemove(&service, &IVehiclesService::vehicleRemoved);

    auto vehicle = new Vehicle(&vehicle::generic, "Some vehicle");

    EXPECT_CALL(vehicles, remove(vehicle)).Times(1);
    service.removeVehicle(vehicle);
    EXPECT_EQ(spyRemove.count(), 1);
}
