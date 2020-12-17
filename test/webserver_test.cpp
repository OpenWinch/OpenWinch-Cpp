/**
 * @file webserver_test.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "controller_mock.hpp"

#include "webserver.hpp"
#include "webserver_config.hpp"
#include <httplib.h>
#include <thread>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

class WebserverTest : public ::testing::Test {
 protected:
  const char *host = "localhost";
  const int port = 8080;

  WebServer *server = nullptr;
  MockWinch *winch = nullptr;

  WebserverTest() { }
  ~WebserverTest() override { }

  void SetUp() override {
    this->winch = new MockWinch();

    EXPECT_CALL(*winch, getState())
      .WillRepeatedly(testing::Return(State::START));

    EXPECT_CALL(*winch, getBattery())
      .WillRepeatedly(testing::Return(10));

    EXPECT_CALL(*winch, getSpeedTarget())
      .WillRepeatedly(testing::Return(30));

    EXPECT_CALL(*winch, start())
      .Times(testing::AnyNumber());

    EXPECT_CALL(*winch, stop())
      .Times(testing::AnyNumber());
    
    EXPECT_CALL(*winch, speedUp(1))
      .Times(testing::AnyNumber());

    EXPECT_CALL(*winch, speedDown(1))
      .Times(testing::AnyNumber());
    
    EXPECT_CALL(*winch, emergency())
      .Times(testing::AnyNumber());
    
    EXPECT_CALL(*winch, initialize())
      .Times(testing::AnyNumber());

    this->server = new WebServer(this->winch);
  }

  void TearDown() override {
    delete this->server;
    delete this->winch;

    this->server = nullptr;
    this->winch = nullptr;
  }

};

TEST_F(WebserverTest, MethodPageRoot) {
  auto thread8080 = std::thread([&]() {
    this->server->getServer()->listen(this->host, this->port);
  });

  while (!this->server->getServer()->is_running()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  // Give GET time to get a few messages.
  std::this_thread::sleep_for(std::chrono::seconds(1));

  httplib::Client cli(this->host, this->port);
  cli.set_connection_timeout(5);

  auto res = cli.Get(HTTP_ROUTE_ROOT);
  ASSERT_TRUE(res);
  EXPECT_EQ(302,  res->status);

  res = cli.Get(HTTP_ROUTE_INDEX);
  ASSERT_TRUE(res);
  EXPECT_EQ(200,  res->status);

  res = cli.Get(HTTP_ROUTE_START);
  ASSERT_TRUE(res);
  EXPECT_EQ(302,  res->status);

  res = cli.Get(HTTP_ROUTE_STOP);
  ASSERT_TRUE(res);
  EXPECT_EQ(302,  res->status);

  res = cli.Get(HTTP_ROUTE_UP);
  ASSERT_TRUE(res);
  EXPECT_EQ(302,  res->status);

  res = cli.Get(HTTP_ROUTE_DOWN);
  ASSERT_TRUE(res);
  EXPECT_EQ(302,  res->status);

  res = cli.Get(HTTP_ROUTE_HALT);
  ASSERT_TRUE(res);
  EXPECT_EQ(302,  res->status);

  this->server->getServer()->stop();
  thread8080.join();
  ASSERT_FALSE(this->server->getServer()->is_running());
}

TEST_F(WebserverTest, MethodPageExtra) {
  auto thread8080 = std::thread([&]() {
    this->server->getServer()->listen(this->host, this->port);
  });

  while (!this->server->getServer()->is_running()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  // Give GET time to get a few messages.
  std::this_thread::sleep_for(std::chrono::seconds(1));

  httplib::Client cli(this->host, this->port);
  cli.set_connection_timeout(2);

  auto res = cli.Get(HTTP_ROUTE_EXTRA);
  ASSERT_TRUE(res);
  EXPECT_EQ(200,  res->status);

  res = cli.Get(HTTP_ROUTE_RESET);
  ASSERT_TRUE(res);
  EXPECT_EQ(302,  res->status);

  res = cli.Get(HTTP_ROUTE_LEFT);
  ASSERT_TRUE(res);
  EXPECT_EQ(302,  res->status);

  res = cli.Get(HTTP_ROUTE_RIGHT);
  ASSERT_TRUE(res);
  EXPECT_EQ(302,  res->status);
  
  this->server->getServer()->stop();
  thread8080.join();
  ASSERT_FALSE(this->server->getServer()->is_running());
}
