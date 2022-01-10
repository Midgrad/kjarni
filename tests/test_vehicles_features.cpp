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

    features.addFeature(vehicle::generic.id, features::dashboard, "GenericDashboard.qml");
    features.addFeature(vehicle::generic.id, "model", "generic.glb");
    features.addFeature("some_specific_uas", "model", "specific.glb");

    EXPECT_EQ(features.features(vehicle::generic.id).count(), 2);
    EXPECT_EQ(features.feature(vehicle::generic.id, features::dashboard), "GenericDashboard.qml");
    EXPECT_EQ(features.feature(vehicle::generic.id, "model"), "generic.glb");
    EXPECT_EQ(features.features("some_specific_uas").count(), 1);
    EXPECT_EQ(features.feature("some_specific_uas", "model"), "specific.glb");
    EXPECT_EQ(spyAdded.count(), 3);
}

TEST_F(VehiclesFeaturesTest, testRemoveFeatures)
{
    // TODO: fixture
    features.addFeature(vehicle::generic.id, features::dashboard, "GenericDashboard.qml");
    features.addFeature(vehicle::generic.id, "model", "generic.glb");
    features.addFeature("some_specific_uas", "model", "specific.glb");

    QSignalSpy spyRemoved(&features, &IVehiclesFeatures::featureRemoved);

    features.removeFeature(vehicle::generic.id, features::dashboard);

    EXPECT_NE(features.feature(vehicle::generic.id, features::dashboard), "GenericDashboard.qml");
    EXPECT_EQ(spyRemoved.count(), 1);
}

TEST_F(VehiclesFeaturesTest, testRemoveType)
{
    // TODO: fixture
    features.addFeature(vehicle::generic.id, features::dashboard, "GenericDashboard.qml");
    features.addFeature(vehicle::generic.id, "model", "generic.glb");
    features.addFeature("some_specific_uas", "model", "specific.glb");

    QSignalSpy spyRemoved(&features, &IVehiclesFeatures::featureRemoved);

    features.removeFeatures(vehicle::generic.id);

    EXPECT_NE(features.feature(vehicle::generic.id, features::dashboard), "GenericDashboard.qml");
    EXPECT_NE(features.feature(vehicle::generic.id, "model"), "generic.glb");
    EXPECT_EQ(spyRemoved.count(), 2);
}
