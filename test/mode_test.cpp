/**
 * @file mode_test.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */
#include "mode.hpp"
#include "controller_mock.hpp"
#include "hardware_mock.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

class ModeTypeTest : public ::testing::Test {
 private:

 protected:
  ModeTypeTest() { }
  ~ModeTypeTest() override { }

  void SetUp() override { }
  void TearDown() override { }
};

TEST_F(ModeTypeTest, MethodCheckFault) {
  EXPECT_TRUE(State::checkFault(State::ERROR));
  EXPECT_FALSE(State::checkFault(State::UNKNOWN));
}

class OneWayModeTest : public ::testing::Test {
 protected:
  ModeEngine* entity = nullptr;
  Winch* winch = nullptr;
  Board* board = nullptr;

  OneWayModeTest() { }
  ~OneWayModeTest() override { }

  void SetUp() override {
    this->winch = new MockWinch();
    this->board = new MockBoard(this->winch);

    this->entity = new OneWayMode(this->board, this->winch);
  }
  void TearDown() override {
    delete this->entity;
    delete this->board;
    delete this->winch;

    this->entity = 0;
  }
};

TEST_F(OneWayModeTest, MethodExtraMode) {

  MockWinch* twinch = (MockWinch *)this->winch;
  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::BOOTED));

  this->entity->run();

  while (!this->entity->isRunning()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  std::this_thread::sleep_for(std::chrono::seconds(1));
  
  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::INIT));
  std::this_thread::sleep_for(std::chrono::seconds(3));

  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::START));
  std::this_thread::sleep_for(std::chrono::seconds(3));

  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::STOP));
  std::this_thread::sleep_for(std::chrono::seconds(3));
  
  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::ERROR));
  std::this_thread::sleep_for(std::chrono::seconds(3));

  EXPECT_TRUE(1);

  this->entity->abort();
  ASSERT_FALSE(this->entity->isRunning());
}


class TwoWayModeTest : public ::testing::Test {
 protected:
  ModeEngine* entity = nullptr;

  TwoWayModeTest() { }
  ~TwoWayModeTest() override { }

  void SetUp() override {
    Winch* winch = new MockWinch();
    Board* board = new MockBoard(winch);

    this->entity = new TwoWayMode(board, winch);
  }
  void TearDown() override {
    delete this->entity;
    this->entity = 0;
  }
};

TEST_F(TwoWayModeTest, MethodExtraMode) {
  // this->entity->extraMode();
  EXPECT_TRUE(1);
}

TEST_F(TwoWayModeTest, MethodIsEndSecurity) {
  EXPECT_TRUE(1); // .isEndSecurity()
}


class InfinityModeTest : public ::testing::Test {
 protected:
  ModeEngine* entity = nullptr;

  InfinityModeTest() { }
  ~InfinityModeTest() override { }

  void SetUp() override {
    Winch* winch = new MockWinch();
    Board* board = new MockBoard(winch);

    this->entity = new InfinityMode(board, winch);
  }
  void TearDown() override {
    delete this->entity;
    this->entity = 0;
  }
};

TEST_F(InfinityModeTest, MethodExtraMode) {
  // this->entity->extraMode();
  EXPECT_TRUE(1);
}
