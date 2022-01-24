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
                                                      false, md::utils::generateId(), name);

    EXPECT_EQ(communicationDescription.type(), type);
    EXPECT_EQ(communicationDescription.linkSpecification().parameters(), params_with_default);
    EXPECT_EQ(communicationDescription.isConnected(), false);
    EXPECT_EQ(communicationDescription.bytesReceived(), 0);
    EXPECT_EQ(communicationDescription.bytesSent(), 0);
    EXPECT_EQ(communicationDescription.protocolSpecification().name(), protocol);
    EXPECT_EQ(communicationDescription.name(), name);
}

TEST(communicationDescriptionTest, testToVariant)
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
                                                      false, md::utils::generateId(), name);

    QVariantMap map;
    map.insert(props::id, communicationDescription.id);
    map.insert(props::name, name);
    map.insert(props::params, params_with_default);
    map.insert(props::state, false);
    map.insert(props::type, type);
    map.insert(props::protocol, protocol);

    EXPECT_EQ(map, communicationDescription.toVariantMap());
}
