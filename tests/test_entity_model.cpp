#include <gtest/gtest.h>

#include <QSignalSpy>

#include "entity.h"
#include "entity_model.h"

using namespace md::domain;
using namespace md::presentation;

using Model = EntityModel<Entity>;

class EntityModelTest : public ::testing::Test
{
public:
    EntityModelTest()
    {
    }

    Model model;
};

TEST_F(EntityModelTest, testEmpty)
{
    EXPECT_EQ(model.data(model.index(0), Model::EntityRole).value<Entity*>(), nullptr);
    EXPECT_EQ(model.data(model.index(0), Model::ParametersRole), QVariant());
}

TEST_F(EntityModelTest, testAddEntities)
{
    Entity entity1;
    entity1.setParameters({ { "property", 686 }, { "flag", "state_1" } });
    model.add(&entity1);

    Entity entity2;
    entity1.setParameters({ { "property", 345 }, { "flag", "state_2" } });
    model.add(&entity2);

    EXPECT_EQ(model.data(model.index(0), Model::EntityRole).value<Entity*>(), &entity1);
    EXPECT_EQ(model.data(model.index(0), Model::ParametersRole).toMap(), entity1.parameters());

    EXPECT_EQ(model.data(model.index(1), Model::EntityRole).value<Entity*>(), &entity2);
    EXPECT_EQ(model.data(model.index(1), Model::ParametersRole).toMap(), entity2.parameters());
}

TEST_F(EntityModelTest, testResetEntities)
{
    Entity entity1;
    entity1.setParameters({ { "property", 686 }, { "flag", "state_1" } });
    model.add(&entity1);

    Entity entity2;
    entity1.setParameters({ { "property", 345 }, { "flag", "state_2" } });
    model.add(&entity2);

    model.reset({ &entity1, &entity2 });

    EXPECT_EQ(model.data(model.index(0), Model::EntityRole).value<Entity*>(), &entity1);
    EXPECT_EQ(model.data(model.index(0), Model::ParametersRole).toMap(), entity1.parameters());

    EXPECT_EQ(model.data(model.index(1), Model::EntityRole).value<Entity*>(), &entity2);
    EXPECT_EQ(model.data(model.index(1), Model::ParametersRole).toMap(), entity2.parameters());
}
