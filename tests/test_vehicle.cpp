#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "vehicle.h"
#include "vehicle_traits.h"

using namespace md::domain;

struct VehicleTestArgs
{
    QVariant id;
    QString name;
    const VehicleType* type;
    QVariantMap params;
};

class VehicleTest : public ::testing::TestWithParam<VehicleTestArgs>
{
public:
    VehicleTest() = default;
};

INSTANTIATE_TEST_SUITE_P(
    instantiation, VehicleTest,
    ::testing::Values(
        VehicleTestArgs({ md::utils::generateId(), "Vehicle 1", &vehicle::generic, {} }),
        VehicleTestArgs({ md::utils::generateId(), "MAV 23", &vehicle::generic,
                          QVariantMap({ { "mav_id", 23 } }) })));

TEST_P(VehicleTest, testConstructFromMap)
{
    VehicleTestArgs args = GetParam();

    QVariantMap map;
    map.insert(props::id, args.id.toString());
    map.insert(props::name, args.name);
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    Vehicle vehicle(args.type, map);

    EXPECT_EQ(vehicle.id, args.id);
    EXPECT_EQ(vehicle.name, args.name);
    EXPECT_EQ(vehicle.type, args.type);
    EXPECT_EQ(vehicle.parameters(), args.params);
}

TEST_P(VehicleTest, testFromVariant)
{
    Vehicle vehicle(&vehicle::generic, QString());

    VehicleTestArgs args = GetParam();

    QVariantMap map;
    map.insert(props::name, args.name);
    map.insert(props::params, QJsonValue::fromVariant(args.params));
    map.insert(props::type, QVariant::fromValue(args.type).toString());

    vehicle.fromVariantMap(map);
    EXPECT_EQ(vehicle.name, args.name);
    EXPECT_EQ(vehicle.type, args.type);
    EXPECT_EQ(vehicle.parameters(), args.params);
}

TEST_P(VehicleTest, testToVariant)
{
    VehicleTestArgs args = GetParam();

    Vehicle vehicle(GetParam().type, GetParam().name);
    vehicle.setParameters(GetParam().params);

    QVariantMap map;
    map.insert(props::id, vehicle.id);
    map.insert(props::name, args.name);
    map.insert(props::params, args.params);
    map.insert(props::type, args.type);
    map.insert(props::online, vehicle.online());

    EXPECT_EQ(map, vehicle.toVariantMap());
}
