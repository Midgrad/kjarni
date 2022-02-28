#include <gtest/gtest.h>

#include <QSignalSpy>

#include "mission_route.h"

using namespace md::domain;

class MissionRouteTest : public ::testing::Test
{
};

TEST_F(MissionRouteTest, testSetCurrent)
{
    MissionRoute route("Test Route", md::utils::generateId());

    route.setCurrent(5);
    EXPECT_EQ(route.currentIndex(), 5);
}
