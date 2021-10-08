#include <gtest/gtest.h>

#include <QSignalSpy>

#include "waypoint.h"

using namespace md::domain;

class WaypointTest : public ::testing::Test
{
};

TEST_F(WaypointTest, testResetParameter)
{
    const Parameter param("Param", Parameter::Real, 17.03);
    const WaypointType type("Type", { param });

    Waypoint wpt("WPT", QString());

    wpt.setParameter("Param", 621.67);
    EXPECT_EQ(wpt.parameter("Param"), 621.67);

    type.resetParameter(&wpt, "Param");
    EXPECT_EQ(wpt.parameter("Param"), 17.03);

    type.resetParameter(&wpt, "Param 2");
    // Nothing to check, just don't fail
}

TEST_F(WaypointTest, testSyncParameters)
{
    const Parameter param1("Param 1", Parameter::Real, 17.03);
    const Parameter param2("Param 2", Parameter::Int, 42);
    const Parameter param3("Param 3", Parameter::Bool, true);

    const WaypointType type1("Type 1", { param1, param2 });
    const WaypointType type2("Type 2", { param2, param3 });

    Waypoint wpt("WPT", QString());
    type1.syncParameters(&wpt);

    EXPECT_EQ(wpt.parameter("Param 1"), 17.03);
    EXPECT_EQ(wpt.parameter("Param 2"), 42);
    EXPECT_FALSE(wpt.parameters().contains("Param 3"));

    wpt.setParameter("Param 2", 33);
    type2.syncParameters(&wpt);

    EXPECT_FALSE(wpt.parameters().contains("Param 1"));
    EXPECT_EQ(wpt.parameter("Param 2"), 33);
    EXPECT_EQ(wpt.parameter("Param 3"), true);
}

TEST_F(WaypointTest, testFromJson)
{
    QJsonObject json;
    json.insert(params::name, "WPT");

    const Parameter param("Param", Parameter::Real);
    const WaypointType type("Type", { param });

    //Waypoint wpt(&type, json.value(params::id).toVariant());
}

//TEST_F(WaypointTest, testToJson)
//{
//    Waypoint wpt;

//    QJsonObject json;
//    json.insert(md::params::id, QJsonValue::fromVariant(args.id));
//    json.insert(md::params::name, args.name);
//    json.insert(md::params::params, QJsonValue::fromVariant(args.params));

//    EXPECT_EQ(json, wpt.toJson());
//}
