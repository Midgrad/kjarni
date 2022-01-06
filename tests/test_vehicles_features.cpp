#include <gtest/gtest.h>

#include <QSignalSpy>

#include "vehicles_features.h"

using namespace testing;
using namespace md::domain;

class VehiclesFeaturesTest : public Test
{
public:
    VehiclesFeatures features;
};

TEST_F(VehiclesFeaturesTest, testAddFeatures)
{
    QSignalSpy spyAdded(&features, &IVehiclesFeatures::featureAdded);

    features.addFeature(vehicleType::generic, features::dashboard, "GenericDashboard.qml");
    features.addFeature(vehicleType::generic, "model", "generic.glb");
    features.addFeature("some_specific_uas", "model", "specific.glb");

    EXPECT_EQ(features.features(vehicleType::generic).count(), 2);
    EXPECT_EQ(features.feature(vehicleType::generic, features::dashboard), "GenericDashboard.qml");
    EXPECT_EQ(features.feature(vehicleType::generic, "model"), "generic.glb");
    EXPECT_EQ(features.features("some_specific_uas").count(), 1);
    EXPECT_EQ(features.feature("some_specific_uas", "model"), "specific.glb");
    EXPECT_EQ(spyAdded.count(), 3);
}

TEST_F(VehiclesFeaturesTest, testRemoveFeatures)
{
    // TODO: fixture
    features.addFeature(vehicleType::generic, features::dashboard, "GenericDashboard.qml");
    features.addFeature(vehicleType::generic, "model", "generic.glb");
    features.addFeature("some_specific_uas", "model", "specific.glb");

    QSignalSpy spyRemoved(&features, &IVehiclesFeatures::featureRemoved);

    features.removeFeature(vehicleType::generic, features::dashboard);

    EXPECT_NE(features.feature(vehicleType::generic, features::dashboard), "GenericDashboard.qml");
    EXPECT_EQ(spyRemoved.count(), 1);
}

TEST_F(VehiclesFeaturesTest, testRemoveType)
{
    // TODO: fixture
    features.addFeature(vehicleType::generic, features::dashboard, "GenericDashboard.qml");
    features.addFeature(vehicleType::generic, "model", "generic.glb");
    features.addFeature("some_specific_uas", "model", "specific.glb");

    QSignalSpy spyRemoved(&features, &IVehiclesFeatures::featureRemoved);

    features.removeFeatures(vehicleType::generic);

    EXPECT_NE(features.feature(vehicleType::generic, features::dashboard), "GenericDashboard.qml");
    EXPECT_NE(features.feature(vehicleType::generic, "model"), "generic.glb");
    EXPECT_EQ(spyRemoved.count(), 2);
}
