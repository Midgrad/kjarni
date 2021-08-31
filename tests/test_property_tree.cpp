#include <gtest/gtest.h>

#include <QSignalSpy>

#include "property_tree.h"

using namespace md::domain;

class PropertyTreeTest : public ::testing::Test
{
public:
    PropertyTreeTest()
    {
    }

    PropertyTree pTree;
};

TEST_F(PropertyTreeTest, testSetProperties)
{
    QSignalSpy nodesSpy(&pTree, &IPropertyTree::rootNodesChanged);
    QSignalSpy propertiesSpy(&pTree, &IPropertyTree::propertiesChanged);

    pTree.setProperties("uav1", { { "altitude", 564 }, { "gs", 45.2 } });
    pTree.setProperties("uav1", { { "altitude", 587 }, { "ias", 46.4 } });
    pTree.setProperties("uav2", { { "altitude", 1296 }, { "gs", 31.7 } });

    EXPECT_EQ(pTree.rootNodes(), QStringList({ "uav1", "uav2" }));

    EXPECT_EQ(pTree.properties("uav1").value("altitude"), 587);
    EXPECT_EQ(pTree.properties("uav1").value("ias"), 46.4);
    EXPECT_FALSE(pTree.properties("uav1").contains("gs"));

    EXPECT_EQ(pTree.properties("uav2").value("altitude"), 1296);
    EXPECT_EQ(pTree.properties("uav2").value("gs"), 31.7);
    EXPECT_FALSE(pTree.properties("uav2").contains("ias"));

    EXPECT_EQ(nodesSpy.count(), 2);
    EXPECT_EQ(propertiesSpy.count(), 3);
}

TEST_F(PropertyTreeTest, testAppendProperties)
{
    QSignalSpy nodesSpy(&pTree, &IPropertyTree::rootNodesChanged);
    QSignalSpy propertiesSpy(&pTree, &IPropertyTree::propertiesChanged);

    pTree.appendProperties("uav1", { { "altitude", 564 }, { "gs", 45.2 } });
    pTree.appendProperties("uav1", { { "altitude", 587 }, { "ias", 46.4 } });
    pTree.appendProperties("uav1", { { "altitude", 1296 }, { "gs", 31.7 } });

    EXPECT_EQ(pTree.rootNodes(), QStringList({ "uav1" }));

    EXPECT_EQ(pTree.properties("uav1").value("altitude"), 1296);
    EXPECT_EQ(pTree.properties("uav1").value("ias"), 46.4);
    EXPECT_EQ(pTree.properties("uav1").value("gs"), 31.7);

    EXPECT_EQ(nodesSpy.count(), 1);
    EXPECT_EQ(propertiesSpy.count(), 3);
}

TEST_F(PropertyTreeTest, testRemoveProperties)
{
    QSignalSpy nodesSpy(&pTree, &IPropertyTree::rootNodesChanged);
    QSignalSpy propertiesSpy(&pTree, &IPropertyTree::propertiesChanged);

    pTree.appendProperties("uav1", { { "altitude", 564 }, { "gs", 45.2 } });
    pTree.removeProperties("uav1", { "altitude" });

    EXPECT_EQ(pTree.properties("uav1").value("gs"), 45.2);
    EXPECT_FALSE(pTree.properties("uav1").contains("altitude"));

    EXPECT_EQ(nodesSpy.count(), 1);
    EXPECT_EQ(propertiesSpy.count(), 2);
}

TEST_F(PropertyTreeTest, testRemoveNode)
{
    QSignalSpy nodesSpy(&pTree, &IPropertyTree::rootNodesChanged);
    QSignalSpy propertiesSpy(&pTree, &IPropertyTree::propertiesChanged);

    pTree.appendProperties("uav1", { { "altitude", 564 }, { "gs", 45.2 } });
    pTree.appendProperties("uav2", { { "altitude", 678 }, { "gs", 27.5 } });
    pTree.removeNode("uav1");

    EXPECT_EQ(pTree.rootNodes(), QStringList({ "uav2" }));
    EXPECT_EQ(pTree.properties("uav2").value("gs"), 27.5);
    EXPECT_TRUE(pTree.properties("uav1").isEmpty());

    EXPECT_EQ(nodesSpy.count(), 3);
    EXPECT_EQ(propertiesSpy.count(), 3);
}
