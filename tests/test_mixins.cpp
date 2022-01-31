#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "dated_mixin.hpp"
#include "named_mixin.hpp"
#include "parametrised_mixin.hpp"

using namespace md::domain;

class MixinsTest : public ::testing::Test
{
};

TEST_F(MixinsTest, testNamedMixin)
{
    NamedMixin<Entity> entity("Name", "id");
    QSignalSpy spyChanged(&entity, &Entity::changed);

    EXPECT_EQ(entity.id, QVariant("id"));
    EXPECT_EQ(entity.name, "Name");
    EXPECT_EQ(spyChanged.count(), 0);

    entity.name.set("New name");

    EXPECT_EQ(entity.name, "New name");
    EXPECT_EQ(spyChanged.count(), 1);

    QVariantMap map = entity.toVariantMap();

    EXPECT_EQ(map.value(props::id), entity.id);
    EXPECT_EQ(map.value(props::name).toString(), entity.name);

    map[props::id] = "id will be ignored";
    map[props::name] = "Newest Name";
    entity.fromVariantMap(map);

    EXPECT_EQ(entity.id, QVariant("id")); // must not change
    EXPECT_EQ(entity.name, map[props::name].toString());
}

TEST_F(MixinsTest, testDatedNamedMixin)
{
    QDateTime now = QDateTime::currentDateTime();
    DatedMixin<NamedMixin<Entity>> entity(now, now, "Name", "id");
    QSignalSpy spyChanged(&entity, &Entity::changed);

    EXPECT_EQ(entity.id, QVariant("id"));
    EXPECT_EQ(entity.name, "Name");
    EXPECT_EQ(entity.createdAt(), now);
    EXPECT_EQ(entity.updatedAt(), now);
    EXPECT_EQ(spyChanged.count(), 0);

    QDateTime other = now.addSecs(1);
    entity.updatedAt.set(other);

    EXPECT_EQ(entity.createdAt(), now);
    EXPECT_EQ(entity.updatedAt(), other);
    EXPECT_EQ(spyChanged.count(), 1);

    QVariantMap map = entity.toVariantMap();

    EXPECT_EQ(map.value(props::id), entity.id);
    EXPECT_EQ(map.value(props::name).toString(), entity.name);
    EXPECT_EQ(map.value(props::createdAt).toDateTime(), entity.createdAt());
    EXPECT_EQ(map.value(props::updatedAt).toDateTime(), entity.updatedAt());

    map[props::id] = "id will be ignored";
    map[props::name] = "Newest Name";
    entity.fromVariantMap(map);

    EXPECT_EQ(entity.id, QVariant("id")); // must not change
    EXPECT_EQ(entity.name, map[props::name].toString());
    EXPECT_EQ(entity.createdAt(), map[props::createdAt]);
    EXPECT_EQ(entity.updatedAt(), map[props::updatedAt]);
}

TEST_F(MixinsTest, testParametrisedMixin)
{
    int changeCounter = 0;

    ParametrisedMixin<Entity> entity(
        QVariantMap(),
        [&changeCounter]() {
            changeCounter++;
        },
        "id");

    entity.setParameters(
        { { "bool_propery", true }, { "int_propery", -42 }, { "string_propery", "test" } });

    EXPECT_EQ(entity.parameter("bool_propery"), true);
    EXPECT_EQ(entity.parameter("int_propery"), -42);
    EXPECT_EQ(entity.parameter("string_propery"), "test");
    EXPECT_EQ(changeCounter, 1);

    entity.setParameter("int_propery", 666);
    EXPECT_EQ(entity.parameter("int_propery"), 666);
    EXPECT_EQ(changeCounter, 2);

    entity.removeParameter("string_propery");
    EXPECT_FALSE(entity.hasParameter("string_propery"));
    EXPECT_EQ(changeCounter, 3);
}
