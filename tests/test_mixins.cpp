#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "dated_mixin.hpp"
#include "named_mixin.hpp"

using namespace md::domain;

class MixinsTest : public ::testing::Test
{
};

TEST_F(MixinsTest, testNamedMixin)
{
    NamedMixin<Entity> entity("Name", "id");
    QSignalSpy spyChanged(&entity, &Entity::changed);

    EXPECT_EQ(entity.name, "Name");
    EXPECT_EQ(entity.id, QVariant("id"));
    EXPECT_EQ(spyChanged.count(), 0);

    entity.name.set("New name");

    EXPECT_EQ(entity.name, "New name");
    EXPECT_EQ(spyChanged.count(), 1);
}

TEST_F(MixinsTest, testDatedNamedMixin)
{
    QDateTime now = QDateTime::currentDateTime();
    DatedMixin<NamedMixin<Entity>> entity(now, now, "Name", "id");
    QSignalSpy spyChanged(&entity, &Entity::changed);

    EXPECT_EQ(entity.updatedAt(), now);
    EXPECT_EQ(entity.createdAt(), now);
    EXPECT_EQ(entity.name, "Name");
    EXPECT_EQ(entity.id, QVariant("id"));
    EXPECT_EQ(spyChanged.count(), 0);

    QDateTime other = now.addSecs(1);
    entity.updatedAt.set(other);

    EXPECT_EQ(entity.createdAt(), now);
    EXPECT_EQ(entity.updatedAt(), other);
    EXPECT_EQ(spyChanged.count(), 1);
}
