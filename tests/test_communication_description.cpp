#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

#include "communication_description.h"

using namespace md::domain;

TEST(communicationDescriptionTest, testConstruct)
{
    QString type = ::link_type::udp;

    QVariantMap params = { { ::link_parameters::type, type }, { ::link_parameters::port, 1 } };
    QVariantMap params_with_default = params.unite(
        { { ::link_parameters::endpoints, {} }, { ::link_parameters::autoResponse, false } });

    QString protocol = "test link";

    LinkSpecification linkSpecification(params);
    ProtocolSpecification protocolSpecification(protocol);

    QString name = "test communication";

    CommunicationDescription communicationDescription(linkSpecification, protocolSpecification,
                                                      false, name);

    EXPECT_EQ(communicationDescription.type(), type);
    EXPECT_EQ(communicationDescription.linkSpecification().parameters(), params_with_default);
    EXPECT_EQ(communicationDescription.isConnected(), false);
    EXPECT_EQ(communicationDescription.bytesReceived(), 0);
    EXPECT_EQ(communicationDescription.bytesSent(), 0);
    EXPECT_EQ(communicationDescription.protocolSpecification().name(), protocol);
    EXPECT_EQ(communicationDescription.name(), name);
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