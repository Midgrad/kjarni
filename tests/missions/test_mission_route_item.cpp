#include <gtest/gtest.h>

#include <QSignalSpy>

#include "mission_route_item.h"

using namespace md::domain;

struct MissionRouteItemTestArgs
{
    QString name;
    ParameterList parameters;
    QVariant id = md::utils::generateId();
};

class MissionRouteItemTest : public ::testing::TestWithParam<MissionRouteItemTestArgs>
{
public:
    MissionRouteItemTest() = default;
};

INSTANTIATE_TEST_SUITE_P(instantiation, MissionRouteItemTest,
                         ::testing::Values(MissionRouteItemTestArgs({
                             "waypoint",
                         })));

TEST_P(MissionRouteItemTest, testToVariant)
{
    MissionRouteItemTestArgs args = GetParam();

    // MissionRouteItem item();
    // TODO: test mission route item
}
