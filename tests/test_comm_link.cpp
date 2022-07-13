#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "comm_link.h"
#include "comm_links_traits.h"

using namespace md::domain;

struct CommLinkTestArgs
{
    QVariant id;
    QString name;
    const CommLinkType* type;
    QString protocol;
    QVariantMap params;
};

class CommLinkTest : public ::testing::TestWithParam<CommLinkTestArgs>
{
public:
    CommLinkTest() = default;
};

INSTANTIATE_TEST_SUITE_P(
    instantiation, CommLinkTest,
    ::testing::Values(
        CommLinkTestArgs({ md::utils::generateId(), "Idle", &comm_link::idle, "", {} }),
        CommLinkTestArgs({ md::utils::generateId(), "UDP", &comm_link::udp, "NMEA",
                           QVariantMap({ { comm_link::port, 17234 },
                                         { comm_link::address, "127.0.0.1" } }) }),
        CommLinkTestArgs({ md::utils::generateId(), "MAV 23", &comm_link::tcp, "Mavlink",
                           QVariantMap({ { comm_link::port, 45753 },
                                         { comm_link::address, "192.168.1.123" } }) })));

TEST_P(CommLinkTest, testConstructFromMap)
{
    CommLinkTestArgs args = GetParam();

    QVariantMap map;
    map.insert(props::id, args.id.toString());
    map.insert(props::name, args.name);
    map.insert(props::params, QJsonValue::fromVariant(args.params));

    CommLink commLink(args.type, map);

    EXPECT_EQ(commLink.id, args.id);
    EXPECT_EQ(commLink.name, args.name);
    EXPECT_EQ(commLink.type, args.type);
    EXPECT_EQ(commLink.parameters(), args.params);
}

TEST_P(CommLinkTest, testFromVariant)
{
    CommLinkTestArgs args = GetParam();
    CommLink commLink(args.type, QString());

    QVariantMap map;
    map.insert(props::name, args.name);
    map.insert(props::params, QJsonValue::fromVariant(args.params));
    map.insert(props::type, QVariant::fromValue(args.type).toString());

    commLink.fromVariantMap(map);
    EXPECT_EQ(commLink.name, args.name);
    EXPECT_EQ(commLink.type, args.type);
    EXPECT_EQ(commLink.parameters(), args.params);
}

TEST_P(CommLinkTest, testToVariant)
{
    CommLinkTestArgs args = GetParam();

    CommLink commLink(GetParam().type, GetParam().name, GetParam().id, GetParam().protocol,
                      GetParam().params);

    QVariantMap map;
    map.insert(props::id, commLink.id);
    map.insert(props::name, args.name);
    map.insert(props::params, args.params);
    map.insert(props::type, args.type);
    map.insert(props::protocol, args.protocol);

    EXPECT_EQ(map, commLink.toVariantMap());
}
