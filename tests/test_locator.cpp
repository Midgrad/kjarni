#include <gtest/gtest.h>

#include <QSignalSpy>

#include "locator.h"

using namespace md::domain;
using namespace md::app;

class IServiceStub : public IService
{
};

class ServiceStub : public IServiceStub
{
};

class LocatorTest : public ::testing::Test
{
};

TEST_F(LocatorTest, testProvideEmpty)
{
    EXPECT_EQ(Locator::get<IServiceStub>(), nullptr);
}

TEST_F(LocatorTest, testProvideAndGet)
{
    ServiceStub stub;
    Locator::provide<IServiceStub>(&stub);

    EXPECT_EQ(Locator::get<IServiceStub>(), &stub);
}

TEST_F(LocatorTest, testUnprovide)
{
    ServiceStub stub;
    Locator::provide<IServiceStub>(&stub);
    Locator::unprovide<IServiceStub>();

    EXPECT_EQ(Locator::get<IServiceStub>(), nullptr);
}
