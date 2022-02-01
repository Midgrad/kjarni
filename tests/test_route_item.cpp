#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "route_item.h"
#include "test_mission_traits.h"
#include "utils.h"

// TODO: impl after plain and complex item refactoring
//using namespace md::domain;

//struct RouteItemTestArgs
//{
//    QVariant id;
//    QString name;
//    QVariantMap position;
//    QVariantMap params;
//    const RouteItemType* type;
//};

//class RouteItemTest : public ::testing::TestWithParam<RouteItemTestArgs>
//{
//public:
//    RouteItemTest() = default;
//};

//INSTANTIATE_TEST_SUITE_P(
//    instantiation, RouteItemTest,
//    ::testing::Values(RouteItemTestArgs({ md::utils::generateId(),
//                                          "WPT 1",
//                                          {},
//                                          { { test_mission::passthrough.id, false } },
//                                          &test_mission::waypoint }),
//                      RouteItemTestArgs({ md::utils::generateId(),
//                                          "WPT 2",
//                                          { { geo::latitude, 54.196783 },
//                                            { geo::longitude, -23.59275 },
//                                            { geo::altitude, 550 } },
//                                          { { test_mission::radius.id, 650 } },
//                                          &test_mission::circle }),
//                      RouteItemTestArgs({ md::utils::generateId(),
//                                          "WPT 3",
//                                          { { geo::latitude, -62.10348 },
//                                            { geo::longitude, 138.74623 },
//                                            { geo::altitude, 550 } },
//                                          { { test_mission::radius.id, 350 } },
//                                          &test_mission::loop })));
