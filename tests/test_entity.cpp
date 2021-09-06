#include <gtest/gtest.h>

#include <QSignalSpy>

#include "entity.h"

using namespace md::domain;

class EntityTest : public ::testing::Test
{
public:
    EntityTest()
    {
    }

    Entity entity;
};

TEST_F(EntityTest, testSetParameters)
{
    QSignalSpy oneSpy(&entity, &Entity::parameterChanged);
    QSignalSpy manySpy(&entity, &Entity::parametersChanged);

    entity.setParameters({ { "param_1", 123 }, { "param_2", 3.123 }, { "param_3", "test" } });

    EXPECT_EQ(entity.parameter("param_1"), 123);
    EXPECT_EQ(entity.parameter("param_2"), 3.123);
    EXPECT_EQ(entity.parameter("param_3"), "test");
    EXPECT_EQ(oneSpy.count(), 3);
    EXPECT_EQ(manySpy.count(), 1);
}

TEST_F(EntityTest, testSetParameter)
{
    QSignalSpy oneSpy(&entity, &Entity::parameterChanged);
    QSignalSpy manySpy(&entity, &Entity::parametersChanged);

    entity.setParameter("param", 123);

    EXPECT_EQ(entity.parameter("param"), 123);
    EXPECT_EQ(oneSpy.count(), 1);
    EXPECT_EQ(manySpy.count(), 1);
}

TEST_F(EntityTest, testRemoveParameter)
{
    entity.setParameters({ { "param_1", 123 }, { "param_2", 3.123 }, { "param_3", "test" } });

    QSignalSpy oneSpy(&entity, &Entity::parameterChanged);
    QSignalSpy manySpy(&entity, &Entity::parametersChanged);

    entity.removeParameter("param_2");

    EXPECT_EQ(entity.parameter("param_1"), 123);
    EXPECT_EQ(entity.parameter("param_2"), QVariant());
    EXPECT_EQ(entity.parameter("param_3"), "test");
    EXPECT_EQ(oneSpy.count(), 1);
    EXPECT_EQ(manySpy.count(), 1);
}
