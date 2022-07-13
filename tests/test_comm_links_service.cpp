#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QSignalSpy>

#include "comm_links_service.h"
#include "comm_links_traits.h"

using namespace testing;
using namespace md::domain;

class CommLinksRepositoryMock : public ICommLinksRepository
{
public:
    MOCK_METHOD(QVariantList, selectCommLinkIds, (), (override));
    MOCK_METHOD(QVariantMap, select, (const QVariant&), (override));

    MOCK_METHOD(void, insert, (CommLink*), (override));
    MOCK_METHOD(void, read, (CommLink*), (override));
    MOCK_METHOD(void, update, (CommLink*), (override));
    MOCK_METHOD(void, remove, (CommLink*), (override));
};

class CommLinksServiceTest : public Test
{
public:
    CommLinksRepositoryMock commLinks;
    CommLinksService service;

    CommLinksServiceTest() : service(&commLinks)
    {
        service.addCommLinkType(&comm_link::udp);
        service.addCommLinkType(&comm_link::tcp);
        service.addCommLinkType(&comm_link::uart);
    }
};

TEST_F(CommLinksServiceTest, testReadAll)
{
    QSignalSpy spyAdded(&service, &ICommLinksService::commLinkAdded);

    EXPECT_CALL(commLinks, selectCommLinkIds()).WillOnce(Return(QVariantList({ "Link 1", 2345 })));
    EXPECT_CALL(commLinks, select(_))
        .WillRepeatedly(Return(QVariantMap({ { props::type, comm_link::udp.id } })));

    service.readAll();

    EXPECT_EQ(service.commLinkIds().count(), 2);
    EXPECT_EQ(service.commLinks().count(), 2);
    EXPECT_EQ(spyAdded.count(), 2);
}

TEST_F(CommLinksServiceTest, testSave)
{
    QSignalSpy spyAdded(&service, &ICommLinksService::commLinkAdded);
    QSignalSpy spyChanged(&service, &ICommLinksService::commLinkChanged);

    auto commLink = new CommLink(&comm_link::tcp, "TCP link");

    EXPECT_CALL(commLinks, insert(commLink)).Times(1);
    service.saveCommLink(commLink);
    EXPECT_EQ(spyAdded.count(), 1);
    EXPECT_EQ(spyChanged.count(), 0);

    EXPECT_CALL(commLinks, update(commLink)).Times(1);
    service.saveCommLink(commLink);
    EXPECT_EQ(spyAdded.count(), 1);
    EXPECT_EQ(spyChanged.count(), 1);
}

TEST_F(CommLinksServiceTest, testRestore)
{
    QSignalSpy spyChanged(&service, &ICommLinksService::commLinkChanged);

    auto commLink = new CommLink(&comm_link::uart, "UART link");

    EXPECT_CALL(commLinks, read(commLink)).Times(1);
    service.restoreCommLink(commLink);
    EXPECT_EQ(spyChanged.count(), 1);
}

TEST_F(CommLinksServiceTest, testRemove)
{
    QSignalSpy spyRemove(&service, &ICommLinksService::commLinkRemoved);

    auto commLink = new CommLink(&comm_link::udp, "UDP link");

    EXPECT_CALL(commLinks, remove(commLink)).Times(1);
    service.removeCommLink(commLink);
    EXPECT_EQ(spyRemove.count(), 1);
}
