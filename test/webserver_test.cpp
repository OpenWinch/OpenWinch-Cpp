/**
 * @file webserver_test.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020-2021
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
  const char* host = "localhost";
  const int port = 8080;

  WebServer* server = nullptr;
  MockWinch* winch = nullptr;
  std::thread* threadHttp = nullptr;

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

  void serverStart() {
    this->threadHttp = new std::thread([&]() {
      this->server->getServer()->listen(this->host, this->port);
    });

    while (!this->server->getServer()->is_running()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // Give GET time to get a few messages.
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  void serverStop() {
    this->server->getServer()->stop();
    this->threadHttp->join();
    ASSERT_FALSE(this->server->getServer()->is_running());
  }

  void testPage(httplib::Client &cli, const char *path, const int httpCode) {
    auto res = cli.Get(path);
    ASSERT_TRUE(res);
    EXPECT_EQ(httpCode,  res->status);
  }

};

TEST_F(WebserverTest, MethodPageRoot) {
  this->serverStart();

  httplib::Client cli(this->host, this->port);
  cli.set_connection_timeout(5);

  this->testPage(cli, HTTP_ROUTE_ROOT, 302);
  this->testPage(cli, HTTP_ROUTE_INDEX, 200);
  this->testPage(cli, HTTP_ROUTE_START, 302);
  this->testPage(cli, HTTP_ROUTE_STOP, 302);
  this->testPage(cli, HTTP_ROUTE_UP, 302);
  this->testPage(cli, HTTP_ROUTE_DOWN, 302);
  this->testPage(cli, HTTP_ROUTE_HALT, 302);

  this->serverStop();
}

TEST_F(WebserverTest, MethodPageExtra) {
  this->serverStart();

  httplib::Client cli(this->host, this->port);
  cli.set_connection_timeout(2);

  this->testPage(cli, HTTP_ROUTE_EXTRA, 200);
  this->testPage(cli, HTTP_ROUTE_RESET, 302);
  this->testPage(cli, HTTP_ROUTE_LEFT, 302);
  this->testPage(cli, HTTP_ROUTE_RIGHT, 302);
  
  this->serverStop();
}
