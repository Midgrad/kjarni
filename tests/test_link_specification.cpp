#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "communication.h"

using namespace md::domain;
//using namespace md::data_source;

struct linkConfigurationTestArgs
{
    QString name;
    QVariantMap params;
};

class linkConfigurationTest : public ::testing::TestWithParam<linkConfigurationTestArgs>
{
public:
    linkConfigurationTest() = default;
};

class linkConfigurationUdpTest : public ::testing::TestWithParam<linkConfigurationTestArgs>
{
public:
    linkConfigurationUdpTest() = default;
};

INSTANTIATE_TEST_SUITE_P(instantiation, linkConfigurationTest,
                         ::testing::Values(linkConfigurationTestArgs({ "Name 2", {} }),
                                           linkConfigurationTestArgs({ "some_name", { {} } }),
                                           linkConfigurationTestArgs({ "link", {} }),
                                           linkConfigurationTestArgs({ "", {} })));

TEST_P(linkConfigurationTest, testConstruct)
{
    linkConfigurationTestArgs args = GetParam();
    Communication linkConfig(GetParam().params, GetParam().name);

    QSignalSpy spy(&linkConfig, &Entity::changed);

    //    EXPECT_EQ(linkConfig.parameters(), QVariantMap());
    EXPECT_EQ(linkConfig.isConnected(), false);
    EXPECT_EQ(linkConfig.bytesReceived(), 0);
    EXPECT_EQ(linkConfig.bytesSent(), 0);
    // TODO: Change to NE after type typification and verification
    EXPECT_EQ(linkConfig.type(), "");
    EXPECT_EQ(spy.count(), 0);
}

INSTANTIATE_TEST_SUITE_P(
    instantiation, linkConfigurationUdpTest,
    ::testing::Values(
        linkConfigurationTestArgs({ "Name 2", { { link_parameters::type, link_type::udp } } }),
        linkConfigurationTestArgs({ "some_name", { { link_parameters::type, link_type::udp } } }),
        linkConfigurationTestArgs({ "link", { { link_parameters::type, link_type::udp } } }),
        linkConfigurationTestArgs({ "", { { link_parameters::type, link_type::udp } } })));

TEST_P(linkConfigurationUdpTest, testUdpConstruct)
{
    linkConfigurationTestArgs args = GetParam();
    Communication linkConfig(GetParam().params, GetParam().name);

    QSignalSpy spy(&linkConfig, &Entity::changed);

    //    EXPECT_EQ(linkConfig.parameters(), args.params);
    EXPECT_EQ(linkConfig.isConnected(), false);
    EXPECT_EQ(linkConfig.bytesReceived(), 0);
    EXPECT_EQ(linkConfig.bytesSent(), 0);
    EXPECT_EQ(linkConfig.parameter(link_parameters::port), 14550);
    EXPECT_EQ(linkConfig.parameter(link_parameters::autoResponse), false);

    EXPECT_EQ(linkConfig.type(), link_type::udp);
    EXPECT_EQ(spy.count(), 0);
}

//TEST_P(BaseModelsTest, testFromVariant)
//{
//    BaseModelsTestArgs args = GetParam();
//    Parametrised entity(args.id, QString());
//
//    QVariantMap map;
//    map.insert(props::name, args.name);
//    map.insert(props::params, QJsonValue::fromVariant(args.params));
//
//    entity.fromVariantMap(map);
//    EXPECT_EQ(entity.id, args.id);
//    EXPECT_EQ(entity.name, args.name);
//    EXPECT_EQ(entity.parameters(), args.params);
//}
//
//TEST_P(BaseModelsTest, testToVariant)
//{
//    BaseModelsTestArgs args = GetParam();
//    Parametrised entity(GetParam().id, GetParam().name, GetParam().params);
//
//    QVariantMap map;
//    map.insert(props::id, args.id);
//    map.insert(props::name, args.name);
//    map.insert(props::params, args.params);
//
//    EXPECT_EQ(map, entity.toVariantMap());
//}