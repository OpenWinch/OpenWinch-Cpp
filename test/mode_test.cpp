/**
 * @file mode_test.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020-2021
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

TEST_F(ModeTypeTest, MethodToString) {
  EXPECT_STRCASEEQ("OneWay", std::string(ModeType(ModeType::OneWay)).c_str());
  EXPECT_STRCASEEQ("TwoWay", std::string(ModeType(ModeType::TwoWay)).c_str());
  EXPECT_STRCASEEQ("Infinity", std::string(ModeType(ModeType::Infinity)).c_str());
}

TEST_F(ModeTypeTest, MethodOperator) {
  EXPECT_TRUE(ModeType(ModeType::OneWay) == ModeType(ModeType::OneWay));
  EXPECT_FALSE(ModeType(ModeType::OneWay) == ModeType(ModeType::Infinity));

  EXPECT_TRUE(ModeType(ModeType::OneWay) != ModeType(ModeType::Infinity));
  EXPECT_FALSE(ModeType(ModeType::OneWay) != ModeType(ModeType::OneWay));
}


class BaseModeTest : public ::testing::Test {
 protected:
  ModeEngine* entity = nullptr;
  MockWinch* winch = nullptr;
  MockBoard* board = nullptr;

  BaseModeTest() { }
  ~BaseModeTest() override { }

  void PostSetUp() {
    this->winch = new MockWinch();
    this->board = new MockBoard(this->winch);

    this->entity = new OneWayMode(this->board, this->winch);

    EXPECT_CALL(*board, getThrottleValue())
      .WillRepeatedly(testing::Return(1));

    EXPECT_CALL(*board, setThrottleValue(testing::_))
      .Times(testing::AnyNumber());

    EXPECT_CALL(*board, initialize())
      .Times(testing::AnyNumber());

    EXPECT_CALL(*board, emergency())
      .Times(testing::AnyNumber());

    EXPECT_CALL(*winch, initialized())
      .Times(testing::AnyNumber());

    EXPECT_CALL(*winch, getSpeedTarget())
      .WillRepeatedly(testing::Return(0));

    EXPECT_CALL(*winch, stop())
      .Times(testing::AnyNumber());

    EXPECT_CALL(*winch, started())
      .Times(testing::AnyNumber());
    
    EXPECT_CALL(*winch, stopped())
      .Times(testing::AnyNumber());
  }
  void TearDown() override {
    delete this->entity;
    delete this->board;
    delete this->winch;

    this->entity = nullptr;
    this->board = nullptr;
    this->winch = nullptr;
  }
};


class OneWayModeTest : public BaseModeTest {
 protected:
  OneWayModeTest() { }
  ~OneWayModeTest() override { }

  void SetUp() override {
    this->winch = new MockWinch();
    this->board = new MockBoard(this->winch);

    this->entity = new OneWayMode(this->board, this->winch);
    this->PostSetUp();
  }
};

TEST_F(OneWayModeTest, MethodExtraMode) {

  MockWinch* twinch = (MockWinch*)this->winch;
  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::BOOTED));

  this->entity->run();

  while (!this->entity->isRunning()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::INIT));
  std::this_thread::sleep_for(std::chrono::milliseconds(300));

  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::START));
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  EXPECT_CALL(*twinch, getSpeedTarget())
    .WillRepeatedly(testing::Return(10));
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));

  EXPECT_CALL(*twinch, getSpeedTarget())
    .WillRepeatedly(testing::Return(5));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::STOP));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  
  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::ERROR));
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  EXPECT_TRUE(1);

  this->entity->abort();
  ASSERT_FALSE(this->entity->isRunning());
}

TEST_F(OneWayModeTest, MethodGetSpeedCurrent) {
  this->entity->getSpeedCurrent();
  EXPECT_TRUE(1);
}

TEST_F(OneWayModeTest, MethodGetDistance) {
  this->entity->getDistance();
  EXPECT_TRUE(1);
}


class TwoWayModeTest : public BaseModeTest {
 protected:
  TwoWayModeTest() { }
  ~TwoWayModeTest() override { }

  void SetUp() override {
    this->winch = new MockWinch();
    this->board = new MockBoard(this->winch);

    this->entity = new TwoWayMode(this->board, this->winch);
    this->PostSetUp();
  }
};


TEST_F(TwoWayModeTest, MethodExtraMode) {

  MockWinch* twinch = (MockWinch *)this->winch;
  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::BOOTED));

  this->entity->run();

  while (!this->entity->isRunning()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::INIT));
  std::this_thread::sleep_for(std::chrono::milliseconds(300));

  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::START));
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  EXPECT_CALL(*twinch, getSpeedTarget())
    .WillRepeatedly(testing::Return(10));
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));

  EXPECT_CALL(*twinch, getSpeedTarget())
    .WillRepeatedly(testing::Return(5));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::STOP));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  
  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::ERROR));
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  EXPECT_TRUE(1);

  this->entity->abort();
  ASSERT_FALSE(this->entity->isRunning());
}

// TEST_F(TwoWayModeTest, MethodIsEndSecurity) {
//   this->entity->isEndSecurity();
//   EXPECT_TRUE(1);
// }


class InfinityModeTest : public BaseModeTest {
 protected:
  InfinityModeTest() { }
  ~InfinityModeTest() override { }

  void SetUp() override {
    this->winch = new MockWinch();
    this->board = new MockBoard(this->winch);

    this->entity = new InfinityMode(this->board, this->winch);
    this->PostSetUp();
  }
};

TEST_F(InfinityModeTest, MethodExtraMode) {

  MockWinch* twinch = (MockWinch *)this->winch;
  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::BOOTED));

  this->entity->run();

  while (!this->entity->isRunning()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::INIT));
  std::this_thread::sleep_for(std::chrono::milliseconds(300));

  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::START));
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  EXPECT_CALL(*twinch, getSpeedTarget())
    .WillRepeatedly(testing::Return(10));
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));

  EXPECT_CALL(*twinch, getSpeedTarget())
    .WillRepeatedly(testing::Return(5));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::STOP));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  
  EXPECT_CALL(*twinch, getState())
    .WillRepeatedly(testing::Return(State::ERROR));
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  EXPECT_TRUE(1);

  this->entity->abort();
  ASSERT_FALSE(this->entity->isRunning());
}